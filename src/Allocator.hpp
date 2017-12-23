#ifndef __ALLOCATOR_HPP__
#define __ALLOCATOR_HPP__
#include "MemorySegment.hpp"

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