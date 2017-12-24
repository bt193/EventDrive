#include "Index.hpp"
#include "Allocators/Allocator.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

Index::Index(Allocator *dataAllocator, Allocator *metadataAllocator)
{
    _dataAllocator = dataAllocator;
    _metadataAllocator = metadataAllocator;
    mkdir("Index", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir("Index/Default", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
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

void Index::LoadData()
{
    struct stat statbuf;
    char filename[512];

    for (auto i = 0;; ++i)
    {
        snprintf(filename, sizeof(filename), "Index/Default/Chunk#%08x.data", i);

        if (stat(filename, &statbuf))
        {
            break;
        }

        printf("Scanning: %s\n", filename);
        auto segment = _dataAllocator->Allocate(filename);

        while (char *mem = segment->Peek(sizeof(int)))
        {
            int op = *(int *)mem;
            printf("Scanning, op: %d\n", op);
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
    char *addr;

    if (!CurrentSegment()->BeginTransaction())
    {
        AddSegment()->BeginTransaction();
    }

    if (!(addr = CurrentSegment()->Peek(length + sizeof(int))))
    {
        AddSegment();
        addr = CurrentSegment()->Peek(length + sizeof(int));
    }

    *(int *) addr = length + sizeof(int);
    memcpy(addr + sizeof(int), memory, length);
    CurrentSegment()->Skip(length + sizeof(int));


    if (!CurrentSegment()->Commit())
    {
        AddSegment()->Commit();
    }

    // std::vector<MemorySegment *> scope;
    // for (auto x : scope)
    // {
    // }
}

MemorySegment *Index::CurrentSegment()
{
    if (!_currentSegment)
    {
        AddSegment();
    }
    return _currentSegment;
}

MemorySegment *Index::AddSegment()
{
    char filename[512];
    int index = _chunks.size();

    snprintf(filename, sizeof(filename), "Index/Default/Chunk#%08x.data", index);
    auto segment = _dataAllocator->Allocate(filename);

    _chunks.push_back(segment);
    _currentSegment = segment;
    return segment;
}