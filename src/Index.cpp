#include "Index.hpp"
#include "Allocators/Allocator.hpp"
#include "MemoryPool.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Config.hpp"
#include <unistd.h>

class Guard
{
  public:
    Guard(void *mem) {}
};

Index::Index(MemoryPool *dataMemoryPool, PositionIndex *positionIndex, EventStreamIndex *eventStreamIndex)
{
    _positionIndex = positionIndex;
    _eventStreamIndex = eventStreamIndex;
    _dataMemoryPool = dataMemoryPool;
    mkdir("Index", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir("Index/Default", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    sha256_init(&_sha256Context);
}

Index::~Index()
{
}

void Index::Scan()
{
    printf("Starting scan...\n");
    LoadData();
    printf("Scan done!\n");
}

char *Hex(char *buffer, char *memory, int length)
{
    int offset = 0;
    const char *table = "0123456789ABCDEF";

    for (int i = 0; i < length; ++i)
    {
        buffer[offset++] = table[(memory[i] >> 4) & 0xF];
        buffer[offset++] = table[memory[i] & 0xF];
    }
    buffer[offset] = '\0';

    return buffer;
}

void Index::LoadData()
{
    struct stat statbuf;
    char filename[512];
    char hash[512];
    char hash2[512];

    for (auto i = 1;; ++i)
    {
        snprintf(filename, sizeof(filename), "Index/Default/Chunk#%08x.data", i);

        if (stat(filename, &statbuf))
        {
            break;
        }

        printf("Scanning: %s\n", filename);

        auto segment = _dataMemoryPool->NextSegment();

        while (char *mem = segment->Peek(sizeof(int)))
        {
            int op = *(int *)mem;
            //printf("Scanning, op: %d\n", op);
            if (op == OpCode::BeginTransaction)
            {
                //printf("BeginTransaction\n");
                segment->Skip(sizeof(int) * 2);
            }
            else if (op == OpCode::Commit)
            {
                //printf("Commit\n");
                segment->Skip(sizeof(int) * 2);
            }
            else if (op > 0)
            {
                Event *event = (Event *)mem;
                sha256_t check;
                char *hashAddr = event->Hash();

                // sha256_update(&_sha256Context, (const BYTE *) event->Hash(), event->Length - sizeof(sha256_t));
                // sha256_final(&_sha256Context, (BYTE *)check);

                // printf("Hash: %s\n", Hex(hash, hashAddr, sizeof(sha256_t)));
                // if (memcmp(check, hashAddr, sizeof(sha256_t)))
                // {
                //     printf("Hash: %s vs. %s\n", Hex(hash, hashAddr, sizeof(sha256_t)), Hex(hash2, check, sizeof(sha256_t)));
                //     assert(!"Invalid hash!");
                // }

                IndexEvent(event);
                //printf("Skip: %d\n", op);
                segment->Skip(op);
                ++_eventCount;
            }
            else if (op == OpCode::None)
            {
                //printf("End!!!\n");
                break;
            }
            else
            {
                //printf("Corrupt\n");
                break;
            }
        }
        _chunks.push_back(segment);
        _currentSegment = segment;
    }
    printf("Total events: %ld\n", _eventCount);
}

bool Index::IsPure(char *memory, int length, int events)
{
    if (events < 1)
    {
        return false;
    }

    int scanedEvents = 0;
    long offset = 0;

    while (offset + sizeof(int) < length)
    {
        auto event = (Event *)&memory[offset];
        scanedEvents += 1;

        offset += event->Length;
    }

    // printf("events: %d vs. %d\n", events, scanedEvents);
    // printf("offset: %ld vs. %d\n", offset, length);
    return events == scanedEvents && offset == length;
}

bool Index::BelongToSameStreamId(char *memory, int length, int events)
{
    auto firstEvent = (Event *)memory;
    auto firstStream = firstEvent->StreamId();

    for (long offset = firstEvent->Length; offset + sizeof(int) < length;)
    {
        auto event = (Event *)&memory[offset];

        if (CompareFixedString((FixedString *)firstStream, (FixedString *)event->StreamId()))
        {
            return false;
        }
        offset += event->Length;
    }

    return true;
}

bool Index::AcceptVersion(Event *event)
{
    if (event->Version == ExpectedVersion::Any)
    {
        return true;
    }

    auto stream = _eventStreamIndex->Lookup(event->StreamId());

    if (event->Version == ExpectedVersion::NoStream)
    {
        return stream == nullptr;
    }
    else if (event->Version == ExpectedVersion::EmptyStream)
    {
        return stream && stream->Version == -1;
    }

    return event->Version == stream->Version + 1;
}

int Index::CompareFixedString(FixedString *str1, FixedString *str2)
{
    return memcmp(str1, str2, str1->Length + sizeof(int));
}

void Index::Put(char *memory, int length, int events)
{
    bool withTransaction = events > 1;
    int token = *(int *)_sha256Context.state;

    if (!IsPure(memory, length, events))
    {
        printf("Unpure data\n");
        _exit(-1);
        return;
    }

    if (!BelongToSameStreamId(memory, length, events))
    {
        printf("BelongToSameStreamId\n");
        _exit(-1);
        return;
    }

    if (!AcceptVersion((Event *)memory))
    {
        printf("AcceptVersion\n");
        _exit(-1);
        return;
    }

    {
        Guard guard(_writeLock);

        if (!AcceptVersion((Event *)memory))
        {
            printf("AcceptVersion\n");
            _exit(-1);
            return;
        }

        if (withTransaction)
        {
            BeginTransaction(token);
        }

        PersistData(memory, length, events);

        if (withTransaction)
        {
            CommitTransaction(token);
        }
    }
}

bool Index::PersistData(char *memory, int length, int events)
{
    for (long offset = 0; offset + sizeof(int) < length; offset += ((Event *)&memory[offset])->Length)
    {
        auto source = (Event *)&memory[offset];
        int len = /*sizeof(int) +*/ source->Length + sizeof(sha256_t);
        auto *dest = (Event *)_dataMemoryPool->Allocate(len);

        memcpy(dest, source, source->Length);
        dest->Length = len;

        // sha256_update(&_sha256Context, (const BYTE *)dest, source->Length);
        // sha256_final(&_sha256Context, (BYTE *)dest->Hash());

        IndexEvent(dest);
        dest->Position = _eventCount++;
    }
    return true;
}

void Index::IndexEvent(Event *event)
{
    auto stream = _eventStreamIndex->Insert(event->StreamId());
    auto index = _positionIndex->Insert(event->Hash());

    stream->Version += 1;
    event->Version = stream->Version;
    if (stream->Version == 0)
    {
        stream->FirstOfStream = index;
    }
    else
    {
        index->PreviousInStream = stream->LastOfStream;
        stream->LastOfStream->NextInStream = index;
    }
    stream->LastOfStream = index;
}

struct Transaction
{
    int Instruction;
    int Token;
};

void Index::BeginTransaction(int token)
{
    PutTransaction(OpCode::BeginTransaction, token);
}

void Index::CommitTransaction(int token)
{
    PutTransaction(OpCode::Commit, token);
}

void Index::PutTransaction(OpCode instruction, int token)
{
    auto *payload = (Transaction *)_dataMemoryPool->Allocate(sizeof(Transaction));

    payload->Instruction = instruction;
    payload->Token = token;
}