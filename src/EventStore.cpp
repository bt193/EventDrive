#include "EventStore.hpp"
#include "Index.hpp"
#include "MemoryPool.hpp"
#include "Allocators/InMemoryAllocator.hpp"
#include "Allocators/MappedFileAllocator.hpp"
#include "Indexes/PositionIndex.hpp"
#include "Indexes/EventStreamIndex.hpp"
#include "Allocators/TemporaryMappedFileAllocator.hpp"

//MappedFileAllocator InMemoryAllocator
EventStore::EventStore()
{
    auto positionMemoryPool = new MemoryPool(new TemporaryMappedFileAllocator());
    auto eventStreamMemoryPool = new MemoryPool(new TemporaryMappedFileAllocator());
    auto dataMemoryPool = new MemoryPool(new MappedFileAllocator());
    auto positionIndex = new PositionIndex(positionMemoryPool);
    auto eventStreamIndex = new EventStreamIndex(eventStreamMemoryPool);

    _index = new Index(dataMemoryPool, positionIndex, eventStreamIndex);
}

EventStore::~EventStore()
{
}

void EventStore::Initialize()
{
    _index->Scan();
}

void EventStore::Put(char *memory, int length)
{
    _index->Put(memory, length);
}