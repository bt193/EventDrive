#include "InMemoryAllocator.hpp"
#include "../Segments/MemorySegment.hpp"
#include <sys/mman.h>
#include <cstdint>
#include <unistd.h>

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

    return Register(new MemorySegment(memory, ChunkSize, length));
}