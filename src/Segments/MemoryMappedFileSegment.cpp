#include "MemoryMappedFileSegment.hpp"

MemoryMappedFileSegment::MemoryMappedFileSegment(char *memory, int length, int fd)
    : MemorySegment(memory, length)
{
    _fd = fd;
}

MemoryMappedFileSegment::~MemoryMappedFileSegment()
{
}