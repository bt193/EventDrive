#ifndef __INDEX_HPP__
#define __INDEX_HPP__
#include <vector>
#include "Indexes/PositionIndex.hpp"
#include "Indexes/EventStreamIndex.hpp"
#include "Crypto/sha256.h"

class Allocator;
class MemorySegment;

struct Event
{
  int Length;
  int Version;
  eventid_t EventId;
  int StreamIdPosition;
  int MetadataPosition;
  int PayloadPosition;
  int EventTypePosition;
  char Data[];
};

class Index
{
public:
  Index(MemoryPool *dataMemoryPool, PositionIndex *positionIndex, EventStreamIndex *eventStreamIndex);
  ~Index();
  void Scan();
  void Put(char *memory, int length);

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
  std::vector<MemorySegment *> _chunks;
  MemorySegment *_currentSegment = nullptr;

  MemoryPool *_dataMemoryPool;
  PositionIndex *_positionIndex;
  EventStreamIndex *_eventStreamIndex;
  SHA256_CTX _sha256Context;
};

#endif
