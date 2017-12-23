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
private:
    Index() = default;
    Index(Index &&) = default;
    Index(const Index &) = default;
    Index &operator=(Index &&) = default;
    Index &operator=(const Index &) = default;
    void LoadData();
    Allocator *_dataAllocator;
    Allocator *_metadataAllocator;
    std::vector<MemorySegment *> _chunks;
};

#endif
