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