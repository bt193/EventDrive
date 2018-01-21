#include "EventStreamIndex.hpp"
#include <string.h>
#include "../Allocators/Allocator.hpp"
#include "../MemoryPool.hpp"
#include <stdio.h>

EventStreamIndex::EventStreamIndex(MemoryPool *memoryPool)
{
    _memoryPool = memoryPool;
}

EventStreamIndex::~EventStreamIndex()
{
}

Stream *EventStreamIndex::Insert(void *key)
{
    return Insert(&_streams, key);
}

Stream *EventStreamIndex::Lookup(void *key)
{
    return Lookup(_streams, key);
}

int EventStreamIndex::CompareNames(char *ptr1, char *ptr2)
{
    int len = *(int *)ptr1;

    return memcmp(ptr1, ptr2, len + sizeof(int));
}

Stream *EventStreamIndex::Lookup(Stream *node, void *id)
{
    while (node)
    {
        int result = CompareNames((char *)id, node->Name);

        if (result == 0)
        {
            return node;
        }
        else if (result < 0)
        {
            node = node->Left;
        }
        else
        {
            node = node->Right;
        }
    }
    return nullptr;
}

Stream *EventStreamIndex::Insert(Stream **node, void *id)
{
    if (*node == nullptr)
    {
        int length = *(int *)id + sizeof(int);
        auto *new_node = (Stream *)_memoryPool->Allocate(sizeof(Stream) + length);

        memcpy(new_node->Name, id, length);
        new_node->Left = nullptr;
        new_node->Right = nullptr;
        new_node->FirstOfStream = nullptr;
        new_node->LastAllocation = nullptr;
        new_node->LastOfStream = nullptr;
        new_node->Version = -1;
        *node = new_node;
        return new_node;
    }

    int result = CompareNames((char *)id, (*node)->Name);

    if (result == 0)
    {
        return *node;
    }
    else if (result > 0)
    {
        return Insert(&(*node)->Right, id);
    }
    else if (result < 0)
    {
        return Insert(&(*node)->Left, id);
    }

    return nullptr;
}

void EventStreamIndex::Delete(void *key)
{
    auto stream = Lookup(key);

    if (!stream)
    {
        return;
    }

    stream->FirstOfStream = nullptr;
    stream->LastOfStream = nullptr;
    stream->Version = -1;
    stream->LastAllocation = nullptr;
}

