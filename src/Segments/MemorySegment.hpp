#ifndef __MEMORYSEGMENT__
#define __MEMORYSEGMENT__

class MemorySegment
{
public:
  MemorySegment(char *memory, int length);
  ~MemorySegment();

protected:
  char *_memory;
  int _length;

private:
  MemorySegment() = default;
  MemorySegment(MemorySegment &&) = default;
  MemorySegment(const MemorySegment &) = default;
  MemorySegment &operator=(MemorySegment &&) = default;
  MemorySegment &operator=(const MemorySegment &) = default;
};

#endif
