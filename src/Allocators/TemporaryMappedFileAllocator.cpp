#include "TemporaryMappedFileAllocator.hpp"
#include "../Segments/MemoryMappedFileSegment.hpp"
#include <stdlib.h>

TemporaryMappedFileAllocator::TemporaryMappedFileAllocator()
{
}

TemporaryMappedFileAllocator::~TemporaryMappedFileAllocator()
{
}

int TemporaryMappedFileAllocator::GetFd(char *input)
{
    return mkstemps("prefixXXXXXXsuffix", 6);
}