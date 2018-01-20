#include "EventCollisionIndex.hpp"
#include <string.h>

EventCollisionIndex::EventCollisionIndex()
{
}

EventCollisionIndex::~EventCollisionIndex()
{
}

void EventCollisionIndex::Insert(eventid_t id, MemoryPool *memoryPool)
{
    Insert(&_root, id, memoryPool);
}

bool EventCollisionIndex::Exists(eventid_t id)
{
    return Exists(_root, id);
}

bool EventCollisionIndex::Exists(EventIdCollisionIndexNode *node, eventid_t id)
{
    while (node && node->EventId)
    {
        int result = memcmp(id, node->EventId, sizeof(eventid_t));

        if (result == 0)
        {
            return true;
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
    return false;
}

void EventCollisionIndex::Insert(EventIdCollisionIndexNode **node, eventid_t id, MemoryPool *memoryPool)
{
    if (*node == nullptr)
    {
        auto *new_node = (EventIdCollisionIndexNode *)memoryPool->AllocateBlock(_pagePointer, sizeof(EventIdCollisionIndexNode));

        memcpy(new_node->EventId, id, sizeof(eventid_t));
        new_node->Left = nullptr;
        new_node->Right = nullptr;
        *node = new_node;
        _pagePointer = (char *)new_node;
        return;
    }

    int result = memcmp(id, (*node)->EventId, sizeof(eventid_t));

    if (result > 0)
    {
        Insert(&(*node)->Right, id, memoryPool);
    }
    else if (result < 0)
    {
        Insert(&(*node)->Left, id, memoryPool);
    }
}
