#ifndef __TEMPORARYMAPPEDFILEALLOCATOR__
#define __TEMPORARYMAPPEDFILEALLOCATOR__
#include "FileAllocator.hpp"

class TemporaryMappedFileAllocator : public FileAllocator
{
public:
  TemporaryMappedFileAllocator();
  virtual ~TemporaryMappedFileAllocator();

protected:
  int GetFd(char *input);

private:
  TemporaryMappedFileAllocator(TemporaryMappedFileAllocator &&) = default;
  TemporaryMappedFileAllocator(const TemporaryMappedFileAllocator &) = default;
  TemporaryMappedFileAllocator &operator=(TemporaryMappedFileAllocator &&) = default;
  TemporaryMappedFileAllocator &operator=(const TemporaryMappedFileAllocator &) = default;
  char _buffer[64];
};

#endif
