#ifndef __EVENTSTREAMINDEX_HPP__
#define __EVENTSTREAMINDEX_HPP__

typedef char sha256_t[32];
typedef char eventid_t[16];
typedef sha256_t position_t;

class MemoryPool;
class Fragment;
class EventStreamIndexNode;

struct Stream
{
    Stream *Left;
    Stream *Right;
    EventStreamIndexNode *FirstOfStream;
    EventStreamIndexNode *LastOfStream;
    int Version;
    void *LastAllocation;
    char Name[];
};

class EventStreamIndex
{
  public:
    EventStreamIndex(MemoryPool *memoryPool);
    ~EventStreamIndex();

    Stream *Insert(void *key);
    Stream *Lookup(void *key);
    void Delete(void *key);

  private:
    EventStreamIndex(EventStreamIndex &&) = default;
    EventStreamIndex(const EventStreamIndex &) = default;
    EventStreamIndex &operator=(EventStreamIndex &&) = default;
    EventStreamIndex &operator=(const EventStreamIndex &) = default;

    Stream *Insert(Stream **node, void *id);
    Stream *Lookup(Stream *node, void *id);
    int CompareNames(char *ptr1, char *ptr2);

    MemoryPool *_memoryPool = nullptr;
    Stream *_streams = nullptr;
};

#endif
