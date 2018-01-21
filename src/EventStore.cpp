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
    _allocator1 = new TemporaryMappedFileAllocator();
    _allocator2 = new TemporaryMappedFileAllocator();
    _allocator3 = new MappedFileAllocator();
    _positionMemoryPool = new MemoryPool(_allocator1);
    _eventStreamMemoryPool = new MemoryPool(_allocator2);
    _dataMemoryPool = new MemoryPool(_allocator3);
    _positionIndex = new PositionIndex(_positionMemoryPool);
    _eventStreamIndex = new EventStreamIndex(_eventStreamMemoryPool);
    _index = new Index(_dataMemoryPool, _positionIndex, _eventStreamIndex);
}

EventStore::~EventStore()
{
    delete _index;
    delete _positionMemoryPool;
    delete _eventStreamMemoryPool;
    delete _dataMemoryPool;
    delete _positionIndex;
    delete _eventStreamIndex;
    delete _allocator1;
    delete _allocator2;
    delete _allocator3;
}

void EventStore::Initialize()
{
    _index->Scan();
}

void EventStore::Put(char *memory, int length, int events)
{
    _index->Put(memory, length, events);
}