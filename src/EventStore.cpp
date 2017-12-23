#include "EventStore.hpp"
#include "Index.hpp"
#include "InMemoryAllocator.hpp"

EventStore::EventStore()
{
    _index = new Index(new InMemoryAllocator(), new InMemoryAllocator());
}

EventStore::~EventStore()
{
}
