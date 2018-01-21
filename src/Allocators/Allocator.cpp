#include "Allocator.hpp"
#include <stdio.h>

Allocator::Allocator()
{
}

Allocator::~Allocator()
{
    for (auto segment : _segments)
    {
        delete segment;
    }
}

MemorySegment *Allocator::Register(MemorySegment *memorySegment)
{
    _segments.push_back(memorySegment);
    return memorySegment;
}