#include "InMemorySegment.hpp"

InMemorySegment::InMemorySegment(char *memory, int length)
    : MemorySegment(memory, length)
{
}

InMemorySegment::~InMemorySegment()
{
}