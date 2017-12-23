#ifndef __ALLOCATOR_HPP__
#define __ALLOCATOR_HPP__
#include "../Segments/MemorySegment.hpp"
#include <unistd.h>

const long PageSize = sysconf(_SC_PAGE_SIZE);
const long ChunkShift = 28;
const long ChunkSize = 1 << ChunkShift; // 256 MiB
const long ChunkMask = ChunkSize - 1;

class Allocator
{
  public:
    Allocator();
    ~Allocator();
    virtual MemorySegment *Allocate(char *input) = 0;

  private:
    Allocator(Allocator &&) = default;
    Allocator(const Allocator &) = default;
    Allocator &operator=(Allocator &&) = default;
    Allocator &operator=(const Allocator &) = default;
};

#endif