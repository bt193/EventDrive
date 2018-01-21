#ifndef __POSITIONINDEX_HPP__
#define __POSITIONINDEX_HPP__

typedef char sha256_t[32];
typedef char eventid_t[16];
typedef sha256_t position_t;

class MemoryPool;
class Event;

struct EventStreamIndexNode
{
  EventStreamIndexNode *Left;
  EventStreamIndexNode *Right;
  EventStreamIndexNode *Next;
  EventStreamIndexNode *NextInStream;
  EventStreamIndexNode *Previous;
  EventStreamIndexNode *PreviousInStream;
  Event *Fragment;
  sha256_t Hash;
};

class PositionIndex
{
public:
  PositionIndex(MemoryPool *memoryPool);
  ~PositionIndex();

  EventStreamIndexNode *Insert(position_t key);
  EventStreamIndexNode *Lookup(position_t key);

private:
  PositionIndex(PositionIndex &&) = default;
  PositionIndex(const PositionIndex &) = default;
  PositionIndex &operator=(PositionIndex &&) = default;
  PositionIndex &operator=(const PositionIndex &) = default;

  EventStreamIndexNode *Lookup(EventStreamIndexNode *node, position_t id);
  EventStreamIndexNode *Insert(EventStreamIndexNode **node, position_t id);

  MemoryPool *_memoryPool = nullptr;
  EventStreamIndexNode *_root = nullptr;
  EventStreamIndexNode *_first = nullptr;
  EventStreamIndexNode *_last = nullptr;
};

/*
class Context
{
  private:
  std::map<void *, void *> _translationMatrix;
};
*/
#endif
