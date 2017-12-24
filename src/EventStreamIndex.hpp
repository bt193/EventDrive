#ifndef __EVENTSTREAMINDEX_HPP__
#define __EVENTSTREAMINDEX_HPP__

typedef char sha256_t[32];
typedef char eventid_t[16];
typedef sha256_t position_t;

class Event;

// [x] EventId
// Keep on same page
struct EventIdCollisionIndexNode
{
    EventIdCollisionIndexNode *Left;
    EventIdCollisionIndexNode *Right;
    eventid_t EventId;
};

// [ ] Position
struct PositionIndexNode
{
    PositionIndexNode *Left;
    PositionIndexNode *Right;
    PositionIndexNode *Next;
    PositionIndexNode *NextInStream;
    position_t Position;
};

// [ ] StreamId
// Sequential
struct Stream;

struct StreamIdIndexNode
{
    StreamIdIndexNode *Left;
    StreamIdIndexNode *Right;
    Stream *Stream;
    char StreamId[];
};

struct Stream
{
    PositionIndexNode *First;
    PositionIndexNode *Last;
    int Version;
};

class EventStreamIndex
{
  public:
    EventStreamIndex();
    ~EventStreamIndex();

  private:
    EventStreamIndex(EventStreamIndex &&) = default;
    EventStreamIndex(const EventStreamIndex &) = default;
    EventStreamIndex &operator=(EventStreamIndex &&) = default;
    EventStreamIndex &operator=(const EventStreamIndex &) = default;
};

#endif
