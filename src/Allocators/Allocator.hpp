#ifndef __ALLOCATOR_HPP__
#define __ALLOCATOR_HPP__
#include "../Segments/MemorySegment.hpp"
#include <unistd.h>
#include <list>

const long PageSize = sysconf(_SC_PAGE_SIZE);
const long ChunkShift = 28;
const long ChunkSize = 1 << ChunkShift; // 256 MiB
const long ChunkMask = ChunkSize - 1;

class Allocator
{
public:
  Allocator();
  virtual ~Allocator();
  virtual MemorySegment *Allocate(char *input) = 0;

protected:
  MemorySegment *Register(MemorySegment *memorySegment);

private:
  Allocator(Allocator &&) = default;
  Allocator(const Allocator &) = default;
  Allocator &operator=(Allocator &&) = default;
  Allocator &operator=(const Allocator &) = default;
  std::list<MemorySegment *> _segments;
};

#endif