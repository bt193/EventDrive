#ifndef __INMEMORYSEGMENT__
#define __INMEMORYSEGMENT__
#include "MemorySegment.hpp"

class InMemorySegment : public MemorySegment
{
  public:
    InMemorySegment(char *memory, int length);
    ~InMemorySegment();

  private:
    InMemorySegment() = default;
    InMemorySegment(InMemorySegment &&) = default;
    InMemorySegment(const InMemorySegment &) = default;
    InMemorySegment &operator=(InMemorySegment &&) = default;
    InMemorySegment &operator=(const InMemorySegment &) = default;
};

#endif
