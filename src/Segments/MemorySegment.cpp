#include "MemorySegment.hpp"

MemorySegment::MemorySegment(char *memory, int length)
{
    _memory = memory;
    _length = length;
}

MemorySegment::~MemorySegment()
{
}
