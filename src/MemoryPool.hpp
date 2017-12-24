#ifndef __MEMORYPOOL__
#define __MEMORYPOOL__
#include "Allocators/Allocator.hpp"

class MemoryPool
{
  public:
    MemoryPool(Allocator *allocator);
    ~MemoryPool();
    char *Allocate(int length);
    char *AllocateBlock(char *ptr, int length);

  private:
    MemoryPool() = default;
    MemoryPool(MemoryPool &&) = default;
    MemoryPool(const MemoryPool &) = default;
    MemoryPool &operator=(MemoryPool &&) = default;
    MemoryPool &operator=(const MemoryPool &) = default;
    MemorySegment *CurrentSegment();
    MemorySegment *NextSegment();
    MemorySegment *_currentSegment = nullptr;
    Allocator *_allocator;
};

#endif
