#include "PositionIndex.hpp"
#include "../MemoryPool.hpp"
#include <string.h>

PositionIndex::PositionIndex(MemoryPool *memoryPool)
{
    _memoryPool = memoryPool;

    position_t start;
    position_t end;

    memset(start, 0, sizeof(position_t));
    memset(end, 0xff, sizeof(position_t));

    _first = Insert(start);
    _last = Insert(end);

    _first->Next = _last;
    _last->Previous = _first;
}

PositionIndex::~PositionIndex()
{
}

EventStreamIndexNode *PositionIndex::Insert(position_t key)
{
    return Insert(&_root, key);
}

EventStreamIndexNode *PositionIndex::Lookup(position_t key)
{
    return Lookup(_root, key);
}

EventStreamIndexNode *PositionIndex::Lookup(EventStreamIndexNode *node, position_t id)
{
    while (node && node->Hash)
    {
        int result = memcmp(id, (char *)node->Hash, sizeof(position_t));

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

EventStreamIndexNode *PositionIndex::Insert(EventStreamIndexNode **node, position_t id)
{
    if (*node == nullptr)
    {
        EventStreamIndexNode *new_node = (EventStreamIndexNode *)_memoryPool->AllocateBlock(nullptr, sizeof(EventStreamIndexNode));

        new_node->Left = nullptr;
        new_node->Right = nullptr;
        new_node->NextInStream = nullptr;
        new_node->PreviousInStream = nullptr;
        memcpy(new_node->Hash, id, sizeof(position_t));
        new_node->Fragment = nullptr;
        *node = new_node;

        if (_last)
        {
            auto previous = _last->Previous;

            previous->Next = new_node;
            new_node->Previous = previous;
            new_node->Next = _last;
            _last->Previous = new_node;
        }
        else
        {
            new_node->Next = nullptr;
            new_node->Previous = nullptr;
        }

        return new_node;
    }

    int result = memcmp(id, (char *)(*node)->Hash, sizeof(position_t));

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