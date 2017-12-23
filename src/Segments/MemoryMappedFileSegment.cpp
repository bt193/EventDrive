#include "MemoryMappedFileSegment.hpp"
#include <unistd.h>

MemoryMappedFileSegment::MemoryMappedFileSegment(char *memory, int length, int fullLength, int fd)
    : MemorySegment(memory, length, fullLength)
{
    _fd = fd;
}

MemoryMappedFileSegment::~MemoryMappedFileSegment()
{
    close(_fd);
}