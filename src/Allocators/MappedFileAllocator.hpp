#ifndef __MAPPEDFILEALLOCATOR__
#define __MAPPEDFILEALLOCATOR__
#include "FileAllocator.hpp"

class MappedFileAllocator : public FileAllocator
{
  public:
    MappedFileAllocator();
    virtual ~MappedFileAllocator();
    MemorySegment *Allocate(char *input);

  private:
    MappedFileAllocator(MappedFileAllocator &&) = default;
    MappedFileAllocator(const MappedFileAllocator &) = default;
    MappedFileAllocator &operator=(MappedFileAllocator &&) = default;
    MappedFileAllocator &operator=(const MappedFileAllocator &) = default;
};

#endif
