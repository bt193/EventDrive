#include "InMemoryAllocator.hpp"
#include <sys/mman.h>
#include <cstdint>
#include <unistd.h>

const long PageSize = sysconf(_SC_PAGE_SIZE);
const long ChunkShift = 28;
const long ChunkSize = 1 << ChunkShift; // 256 MiB
const long ChunkMask = ChunkSize - 1;

InMemoryAllocator::InMemoryAllocator()
{
}

InMemoryAllocator::~InMemoryAllocator()
{
}

MemorySegment *InMemoryAllocator::Allocate(char *input)
{
    size_t length = ChunkSize + PageSize;
    char *memory = (char *)mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    mprotect(memory + ChunkSize, PageSize, PROT_NONE);


    return nullptr;
}