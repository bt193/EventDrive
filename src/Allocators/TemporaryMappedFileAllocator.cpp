#include "TemporaryMappedFileAllocator.hpp"
#include "../Segments/MemoryMappedFileSegment.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

TemporaryMappedFileAllocator::TemporaryMappedFileAllocator()
{
}

TemporaryMappedFileAllocator::~TemporaryMappedFileAllocator()
{
}

int TemporaryMappedFileAllocator::GetFd(char *input)
{
    char buffer[] = "temp.XXXXXX";

    auto fd = mkstemp((char *)buffer);
    unlink(buffer);
    return fd;
}