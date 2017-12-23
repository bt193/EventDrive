#include "Index.hpp"
#include "Allocators/Allocator.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

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
    LoadData();
}

void Index::LoadData()
{
    struct stat statbuf;
    char filename[512];

    for (auto i = 0;; ++i)
    {
        if (stat(filename, &statbuf))
        {
            break;
        }

        snprintf(filename, sizeof(filename), "Index/Default/Chunk#%08x.data", i);
        auto segment = _dataAllocator->Allocate(filename);

        _chunks.push_back(segment);
    }
}

void Index::Put(char *memory, int length)
{
    // std::vector<MemorySegment *> scope;
    if (!CurrentSegment()->Put(memory, length))
    {
        AddSegment();
        CurrentSegment()->Put(memory, length);
    }

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