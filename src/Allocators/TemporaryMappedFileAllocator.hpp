#ifndef __TEMPORARYMAPPEDFILEALLOCATOR__
#define __TEMPORARYMAPPEDFILEALLOCATOR__
#include "FileAllocator.hpp"

class TemporaryMappedFileAllocator : public FileAllocator
{
  public:
    TemporaryMappedFileAllocator();
    virtual ~TemporaryMappedFileAllocator();

  private:
    TemporaryMappedFileAllocator(TemporaryMappedFileAllocator &&) = default;
    TemporaryMappedFileAllocator(const TemporaryMappedFileAllocator &) = default;
    TemporaryMappedFileAllocator &operator=(TemporaryMappedFileAllocator &&) = default;
    TemporaryMappedFileAllocator &operator=(const TemporaryMappedFileAllocator &) = default;
};

#endif
