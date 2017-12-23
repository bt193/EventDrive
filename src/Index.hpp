#ifndef __INDEX_HPP__
#define __INDEX_HPP__
#include <vector>

class Allocator;
class MemorySegment;

class Index
{
  public:
    Index(Allocator *dataAllocator, Allocator *metadataAllocator);
    ~Index();
    void Scan();
    void Put(char *memory, int length);

  private:
    Index() = default;
    Index(Index &&) = default;
    Index(const Index &) = default;
    Index &operator=(Index &&) = default;
    Index &operator=(const Index &) = default;

    MemorySegment *CurrentSegment();
    MemorySegment *AddSegment();

    void LoadData();
    Allocator *_dataAllocator;
    Allocator *_metadataAllocator;
    std::vector<MemorySegment *> _chunks;
    MemorySegment *_currentSegment = nullptr;
};

#endif
