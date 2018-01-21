#ifndef __INDEX_HPP__
#define __INDEX_HPP__
#include <vector>
#include "Indexes/PositionIndex.hpp"
#include "Indexes/EventStreamIndex.hpp"
#include "Crypto/sha256.h"
#include <stdio.h>

class Allocator;
class MemorySegment;

struct Event
{
  int Length;
  int Version;
  eventid_t EventId;
  int StreamIdOffset;
  int MetadataOffset;
  int PayloadOffset;
  char Data[];

  char *EventType()
  {
    return Data;
  }

  char *StreamId()
  {
    //printf("StreamIdOffset: %d\n", StreamIdOffset);
    return &Data[StreamIdOffset];
  }

  char *Metadata()
  {
    return &Data[MetadataOffset];
  }

  char *Payload()
  {
    return &Data[PayloadOffset];
  }

  char *Hash()
  {
    return (char *)this + Length - sizeof(sha256_t);
  }
};

class Index
{
public:
  Index(MemoryPool *dataMemoryPool, PositionIndex *positionIndex, EventStreamIndex *eventStreamIndex);
  ~Index();
  void Scan();
  void Put(char *memory, int length, int events = 1);

private:
  Index() = default;
  Index(Index &&) = default;
  Index(const Index &) = default;
  Index &operator=(Index &&) = default;
  Index &operator=(const Index &) = default;

  void BeginTransaction();
  void CommitTransaction();
  void InjectData(char *memory, int length);
  void LoadData();
  void IndexEvent(Event *event);
  std::vector<MemorySegment *> _chunks;
  MemorySegment *_currentSegment = nullptr;

  MemoryPool *_dataMemoryPool;
  PositionIndex *_positionIndex;
  EventStreamIndex *_eventStreamIndex;
  SHA256_CTX _sha256Context;
  long _eventCount = 0;
};

#endif
