#include "FileAllocator.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "../Segments/MemoryMappedFileSegment.hpp"

FileAllocator::FileAllocator()
{
}

FileAllocator::~FileAllocator()
{
    munmap(_memory, ChunkSize);
    close(_fd);
}

MemorySegment *FileAllocator::Allocate(char *input)
{
    _fd = GetFd(input);
    auto length = ChunkSize + PageSize;

    if (lseek(_fd, 0, SEEK_END) != length)
    {
        char empty[] = {'\0'};

        lseek(_fd, length - 1, SEEK_SET);
        write(_fd, empty, sizeof(empty));
    }
    lseek(_fd, 0, SEEK_SET);
    _memory = (char *)mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0);
    mprotect(_memory + ChunkSize, PageSize, PROT_NONE);
    return Register(new MemoryMappedFileSegment(_memory, ChunkSize, length, _fd));
}