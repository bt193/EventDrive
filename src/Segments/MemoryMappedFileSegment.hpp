#ifndef __MEMORYMAPPEDFILESEGMENT__
#define __MEMORYMAPPEDFILESEGMENT__
#include "MemorySegment.hpp"

class MemoryMappedFileSegment : public MemorySegment
{
  public:
    MemoryMappedFileSegment(char *memory, int length, int fd);
    ~MemoryMappedFileSegment();

  private:
    MemoryMappedFileSegment();
    MemoryMappedFileSegment(MemoryMappedFileSegment &&) = default;
    MemoryMappedFileSegment(const MemoryMappedFileSegment &) = default;
    MemoryMappedFileSegment &operator=(MemoryMappedFileSegment &&) = default;
    MemoryMappedFileSegment &operator=(const MemoryMappedFileSegment &) = default;
    int _fd;
};

#endif
