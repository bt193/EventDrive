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

void EventStore::Initialize()
{
    _index->Scan();
}

void EventStore::Put(char *memory, int length)
{
    _index->Put(memory, length);
}