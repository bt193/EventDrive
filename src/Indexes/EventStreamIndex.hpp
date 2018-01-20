#ifndef __EVENTSTREAMINDEX_HPP__
#define __EVENTSTREAMINDEX_HPP__

typedef char sha256_t[32];
typedef char eventid_t[16];
typedef sha256_t position_t;

class MemoryPool;

// class Stream;
// template <typename T>
// struct BTreeNode
// {
//     T *Left;
//     T *Right;
// };

// struct EventIdBTreeNode : public BTreeNode<EventIdBTreeNode>
// {
//     eventid_t EventId;
// };

// template <typename T>
// class BTreeIndex
// {
//     // insert
//     // lookup
//     virtual T *Allocate() = 0;
// };

// class EventIdIndex : public BTreeIndex<EventIdBTreeNode>
// {
//   public:
//     EventIdBTreeNode *Allocate() override
//     {
//         return nullptr;
//     }
// };

struct EventStreamIndexNode// : public BTreeNode<EventStreamIndexNode>
{
    EventStreamIndexNode *Left;
    EventStreamIndexNode *Right;
    EventStreamIndexNode *Next;
    EventStreamIndexNode *NextInStream;
    EventStreamIndexNode *Previous;
    EventStreamIndexNode *PreviousInStream;
    sha256_t Hash;
};

struct Stream// : public BTreeNode<EventStreamIndexNode>
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
    EventStreamIndexNode *_hej = nullptr;
};

#endif
