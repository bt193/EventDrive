#ifndef __INMEMORYALLOCATOR__
#define __INMEMORYALLOCATOR__
#include "Allocator.hpp"

class InMemoryAllocator : public Allocator
{
  public:
    InMemoryAllocator();
    ~InMemoryAllocator();
    MemorySegment *Allocate(char *input);

  private:
    InMemoryAllocator(InMemoryAllocator &&) = default;
    InMemoryAllocator(const InMemoryAllocator &) = default;
    InMemoryAllocator &operator=(InMemoryAllocator &&) = default;
    InMemoryAllocator &operator=(const InMemoryAllocator &) = default;
};

#endif
