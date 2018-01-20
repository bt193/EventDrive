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
    MemorySegment *NextSegment();

  private:
    MemoryPool() = default;
    MemoryPool(MemoryPool &&) = default;
    MemoryPool(const MemoryPool &) = default;
    MemoryPool &operator=(MemoryPool &&) = default;
    MemoryPool &operator=(const MemoryPool &) = default;
    MemorySegment *CurrentSegment();
    MemorySegment *_currentSegment = nullptr;
    Allocator *_allocator;
    int _segments = 0;
};

#endif
