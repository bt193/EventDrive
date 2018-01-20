#include "MappedFileAllocator.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

MappedFileAllocator::MappedFileAllocator()
{
}

MappedFileAllocator::~MappedFileAllocator()
{
}

int MappedFileAllocator::GetFd(char *input)
{
    printf("[%s]\n", input);
    return open(input, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
}