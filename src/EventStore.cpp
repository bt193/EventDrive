#include "EventStore.hpp"
#include "Index.hpp"
#include "Allocators/InMemoryAllocator.hpp"
#include "Allocators/MappedFileAllocator.hpp"

EventStore::EventStore()
{
    _index = new Index(new MappedFileAllocator(), new InMemoryAllocator());
}

EventStore::~EventStore()
{
}
