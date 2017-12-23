#include "EventStore.hpp"
#include "Index.hpp"
#include "InMemoryAllocator.hpp"
#include "MappedFileAllocator.hpp"

EventStore::EventStore()
{
    _index = new Index(new MappedFileAllocator(), new InMemoryAllocator());
}

EventStore::~EventStore()
{
}
