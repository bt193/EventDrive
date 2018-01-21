#include "TemporaryMappedFileAllocator.hpp"
#include "../Segments/MemoryMappedFileSegment.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

TemporaryMappedFileAllocator::TemporaryMappedFileAllocator()
{
    strncpy(_buffer, "temp.XXXXXX", sizeof(_buffer));
}

TemporaryMappedFileAllocator::~TemporaryMappedFileAllocator()
{
    // unlink(_buffer);
}

int TemporaryMappedFileAllocator::GetFd(char *input)
{
    auto fd = mkstemp(_buffer);

    unlink(_buffer);
    return fd;
}