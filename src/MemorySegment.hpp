#ifndef __MEMORYSEGMENT__
#define __MEMORYSEGMENT__

class MemorySegment
{
public:
    MemorySegment();
    ~MemorySegment();
private:
    MemorySegment(MemorySegment &&) = default;
    MemorySegment(const MemorySegment &) = default;
    MemorySegment &operator=(MemorySegment &&) = default;
    MemorySegment &operator=(const MemorySegment &) = default;
};

#endif
