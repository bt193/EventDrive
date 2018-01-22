#include "MemoryPool.hpp"
#include <stdio.h>

MemoryPool::MemoryPool(Allocator *allocator)
{
    _allocator = allocator;
}

MemoryPool::~MemoryPool()
{
}

char *MemoryPool::Allocate(int length)
{
    char *addr = CurrentSegment()->Peek(length);

    if (!addr)
    {
        addr = NextSegment()->Peek(length);
    }

    CurrentSegment()->Skip(length);
    return addr;
}

char *MemoryPool::AllocateBlock(char *ptr, int length)
{
    const long PageSize = 4096;
    const long PageMask = ~(PageSize - 1);
    long addr = (long)ptr;

    if (!ptr || (addr & PageMask) != ((addr + length) & PageMask))
    {
        return Allocate(4096);
    }

    return (char *)(addr + length);
}

MemorySegment *MemoryPool::CurrentSegment()
{
    if (!_currentSegment)
    {
        NextSegment();
    }
    return _currentSegment;
}

MemorySegment *MemoryPool::NextSegment()
{
    char buffer[4096];

    snprintf(buffer, sizeof(buffer), "Index/Default/Chunk#%08x.data", _segments++);
    return _currentSegment = _allocator->Allocate(buffer);
}