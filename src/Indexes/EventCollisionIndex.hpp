#ifndef __EVENTCOLLISIONINDEX_HPP__
#define __EVENTCOLLISIONINDEX_HPP__
#include "../MemoryPool.hpp"

typedef char eventid_t[16];

struct EventIdCollisionIndexNode
{
    EventIdCollisionIndexNode *Left;
    EventIdCollisionIndexNode *Right;
    eventid_t EventId;
};

class EventCollisionIndex
{
  public:
    EventCollisionIndex();
    ~EventCollisionIndex();
    void Insert(eventid_t id, MemoryPool *memoryPool);
    bool Exists(eventid_t id);

  private:
    EventCollisionIndex(EventCollisionIndex &&) = default;
    EventCollisionIndex(const EventCollisionIndex &) = default;
    EventCollisionIndex &operator=(EventCollisionIndex &&) = default;
    EventCollisionIndex &operator=(const EventCollisionIndex &) = default;
    bool Exists(EventIdCollisionIndexNode *node, eventid_t id);
    void Insert(EventIdCollisionIndexNode **node, eventid_t id, MemoryPool *memoryPool);
    EventIdCollisionIndexNode *_root = nullptr;
    char *_pagePointer = nullptr;
};

#endif
