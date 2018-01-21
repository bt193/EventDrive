#include "Index.hpp"
#include "Allocators/Allocator.hpp"
#include "MemoryPool.hpp"
#include "Config.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

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
                segment->Skip(sizeof(int));
            }
            else if (op == OpCode::Commit)
            {
                //printf("Commit\n");
                segment->Skip(sizeof(int));
            }
            else if (op > 0)
            {
                sha256_t check;
                char *hashAddr = mem + op - sizeof(sha256_t);

                sha256_update(&_sha256Context, (const BYTE *)mem, op - sizeof(sha256_t));
                sha256_final(&_sha256Context, (BYTE *)check);

                printf("Hash: %s\n", Hex(hash, hashAddr, sizeof(sha256_t)));
                if (memcmp(check, hashAddr, sizeof(sha256_t)))
                {
                    printf("Hash: %s vs. %s\n", Hex(hash, hashAddr, sizeof(sha256_t)), Hex(hash2, check, sizeof(sha256_t)));
                    assert(!"Invalid hash!");
                }

                //_eventStreamIndex->Insert(((Event *) mem)->EventId);
                _positionIndex->Insert(hashAddr);

                //printf("Skip: %d\n", op);
                segment->Skip(op);
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
}

void Index::Put(char *memory, int length)
{
    Event *event = (Event *)memory;
    char *ptr = event->Data + *(int *)event->Data + sizeof(int);

    auto stream = _eventStreamIndex->Lookup(ptr);

    if (stream)
    {
        printf("Stream: %p, version: %d\n", stream, stream->Version);
    }

    if (event->Version >= 0)
    {
        if (stream)
        {
            if (stream->Version + 1 != event->Version)
            {
                printf("Expected version: %d, got: %d\n", stream->Version + 1, event->Version);
                return;
            }
        }
        else if (event->Version != 0)
        {
            printf("(!stream) Expected version: 0, got: %d\n", event->Version);
            return;
        }
    }
    else if (event->Version == ExpectedVersion::EmptyStream)
    {
        if (!stream)
        {
            printf("Stream does not exists\n");
            return;
        }
        else if (stream->Version != -1)
        {
            printf("Stream is not empty\n");
            return;
        }
    }
    else if (event->Version == ExpectedVersion::NoStream)
    {
        if (stream)
        {
            printf("Stream exists\n");
            return;
        }
    }
    else if (event->Version == ExpectedVersion::Any)
    {
    }

    printf("-- Ok version: %d\n", event->Version);

    BeginTransaction();
    InjectData(memory, length);
    CommitTransaction();
}

void Index::InjectData(char *memory, int length)
{
    int len = sizeof(int) + length + sizeof(sha256_t);
    char *addr = _dataMemoryPool->Allocate(len);

    *(int *)addr = len;
    memcpy(addr + sizeof(int), memory, length);
    sha256_update(&_sha256Context, (const BYTE *)addr, sizeof(int) + length);
    sha256_final(&_sha256Context, (BYTE *)addr + sizeof(int) + length);

    auto data = ((Event *)memory)->Data;
    auto name = data + *(int *)data + sizeof(int);

    auto stream = _eventStreamIndex->Insert(name);
    auto index = _positionIndex->Insert(addr + sizeof(int) + length);

    stream->Version += 1;
}

void Index::BeginTransaction()
{
    int *ptr = (int *)_dataMemoryPool->Allocate(sizeof(OpCode));

    *ptr = OpCode::BeginTransaction;
}

void Index::CommitTransaction()
{
    int *ptr = (int *)_dataMemoryPool->Allocate(sizeof(OpCode));

    *ptr = OpCode::Commit;
}
