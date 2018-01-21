#ifndef __FILEALLOCATOR__
#define __FILEALLOCATOR__
#include "Allocator.hpp"

class FileAllocator : public Allocator
{
public:
  FileAllocator();
  virtual ~FileAllocator();
  MemorySegment *Allocate(char *input);

protected:
  virtual int GetFd(char *input) = 0;

private:
  FileAllocator(FileAllocator &&) = default;
  FileAllocator(const FileAllocator &) = default;
  FileAllocator &operator=(FileAllocator &&) = default;
  FileAllocator &operator=(const FileAllocator &) = default;

  int _fd;
  char *_memory;
};

#endif
