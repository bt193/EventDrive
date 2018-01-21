#ifndef __EVENTSTORE_HPP__
#define __EVENTSTORE_HPP__
#include "Config.hpp"

class Index;
class MemoryPool;
class PositionIndex;
class EventStreamIndex;
class Allocator;

class EventStore
{
public:
  EventStore();
  ~EventStore();
  void Initialize();

  void Put(char *memory, int length, int events);

  int ReadForward(position_t fromPosition, char *buffer, int length, int maxEvents);
  int ReadBackwards(position_t fromPosition, char *buffer, int length, int maxEvents);
  int ReadStreamForward(FixedString *name, char *buffer, int length, int minVersion, int maxVersion);
  int ReadStreamBackwards(FixedString *name, char *buffer, int length, int minVersion, int maxVersion);

private:
  EventStore(EventStore &&) = default;
  EventStore(const EventStore &) = default;
  EventStore &operator=(EventStore &&) = default;
  EventStore &operator=(const EventStore &) = default;
  Index *_index;

  MemoryPool *_positionMemoryPool;
  MemoryPool *_eventStreamMemoryPool;
  MemoryPool *_dataMemoryPool;
  PositionIndex *_positionIndex;
  EventStreamIndex *_eventStreamIndex;
  Allocator *_allocator1;
  Allocator *_allocator2;
  Allocator *_allocator3;
};

#endif