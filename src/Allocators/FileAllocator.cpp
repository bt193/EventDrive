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
}

MemorySegment *FileAllocator::Allocate(char *input)
{
    auto fd = GetFd(input);
    auto length = ChunkSize + PageSize;

    if (lseek(fd, 0, SEEK_END) != ChunkSize)
    {
        char empty[] = {'\0'};

        lseek(fd, ChunkSize - 1, SEEK_SET);
        write(fd, empty, sizeof(empty));
    }
    lseek(fd, 0, SEEK_SET);
    auto memory = (char *)mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    mprotect(memory + ChunkSize, PageSize, PROT_NONE);
    return new MemoryMappedFileSegment(memory, ChunkSize, length, fd);
}