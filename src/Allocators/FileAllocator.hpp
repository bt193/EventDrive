#ifndef __FILEALLOCATOR__
#define __FILEALLOCATOR__
#include "Allocator.hpp"

class FileAllocator : public Allocator
{
  public:
    FileAllocator();
    virtual ~FileAllocator();
    virtual MemorySegment *Allocate(char *input) = 0;

  private:
    FileAllocator(FileAllocator &&) = default;
    FileAllocator(const FileAllocator &) = default;
    FileAllocator &operator=(FileAllocator &&) = default;
    FileAllocator &operator=(const FileAllocator &) = default;
};

#endif
