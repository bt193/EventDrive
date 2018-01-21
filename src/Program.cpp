#include "EventStore.hpp"

#include <stdio.h>
#include <string.h>

#include "Random.hpp"

class Event;

void TestEventStreamIndex();
void TestEventCollisionIndex();
void TestPositionIndex();
int CreateEvent(char *buffer, int len, eventid_t eventId, const char *streamId, int version, const char *metadata, const char *payload, const char *eventType);

#include <stdlib.h>
#include "Index.hpp"
#include "Config.hpp"

int main(int argc, char *argv[])
{
    Random random;
    eventid_t eventId1;
    eventid_t eventId2;
    eventid_t eventId3;

    random.Scramble(eventId1, sizeof(eventid_t));
    random.Scramble(eventId2, sizeof(eventid_t));
    random.Scramble(eventId3, sizeof(eventid_t));

    // bzero(eventId1, sizeof(eventid_t));
    // bzero(eventId2, sizeof(eventid_t));
    // bzero(eventId3, sizeof(eventid_t));
    // CreateEvent(eventId, "player/adam", 0, "{}", "{}", "ESPlus.SuperEvent");
    // return 0;
#if false
    TestEventCollisionIndex();
    TestEventStreamIndex();
    TestPositionIndex();
#else
    char buffer[40960];

    bzero(buffer, sizeof(buffer));
    EventStore eventStore;

    eventStore.Initialize();

    int len;
    random.Scramble(eventId1, sizeof(eventid_t));
    auto res1 = CreateEvent(buffer, sizeof(buffer), eventId1, "player/1", ExpectedVersion::Any, "{0000000000000000000000000000000000000000000000}", "{}", "ESPlus.SuperEvent");
    auto res2 = CreateEvent(buffer + res1, sizeof(buffer) - res1, eventId1, "player/1", ExpectedVersion::Any, "{0000000000000000000000000000000000000000000000}", "{}", "ESPlus.SuperEvent");

    for (int i = 0; i < 5000; ++i)
    {
        eventStore.Put(buffer, res1 + res2, 2);
        break;
    }
    //len = CreateEvent(buffer, sizeof(buffer), eventId1, "player/1", 1, "{}", "{}", "ESPlus.SuperEvent");
    //eventStore.Put(buffer, len - sizeof(sha256_t));

    // len = CreateEvent(buffer, sizeof(buffer), eventId1, "player/1", 1, "{}", "{}", "ESPlus.SuperEvent");
    // eventStore.Put(buffer + sizeof(int), len - sizeof(int) - sizeof(sha256_t));
    // for (auto i = 0; i < 3; ++i)
    // {
    //     eventStore.Put(buffer + sizeof(int), len - sizeof(int) - sizeof(sha256_t));
    // }

    printf("Waiting...\n");
    //getchar();
#endif
    printf("Done!\n");

    return 0;
}

#include "Indexes/EventCollisionIndex.hpp"
#include "Allocators/InMemoryAllocator.hpp"
#include <assert.h>

void TestEventCollisionIndex()
{
    printf("TestEventCollisionIndex\n");
    auto random = new Random();
    auto allocator = new InMemoryAllocator();
    auto pool = new MemoryPool(allocator);
    auto index = new EventCollisionIndex();
    eventid_t id1, id2, id3;

    random->Scramble((char *)&id1, sizeof(eventid_t));
    random->Scramble((char *)&id2, sizeof(eventid_t));
    random->Scramble((char *)&id3, sizeof(eventid_t));

    {
        printf("* does not exist\n");
        assert(!index->Exists(id1));
    }

    {
        printf("* exists after insert\n");
        index->Insert(id1, pool);

        assert(index->Exists(id1));
    }

    {
        printf("* insert existing\n");
        index->Insert(id1, pool);
    }

    {
        printf("* insert another\n");
        index->Insert(id2, pool);

        assert(index->Exists(id1));
        assert(index->Exists(id2));
        assert(!index->Exists(id3));
    }

    {
        printf("* modify search memory\n");
        random->Scramble((char *)&id1, sizeof(eventid_t));

        assert(!index->Exists(id1));
    }
    printf("TestEventCollisionIndex... Done!\n");
}

#include "Indexes/EventStreamIndex.hpp"

void WriteToBuffer(char *buffer, const char *text)
{
    int length = strlen(text);

    *(int *)buffer = length;
    memcpy(buffer + sizeof(int), text, length);
}

void TestEventStreamIndex()
{
    printf("TestEventStreamIndex\n");
    auto random = new Random();
    auto allocator = new InMemoryAllocator();
    auto pool = new MemoryPool(allocator);
    auto index = new EventStreamIndex(pool);
    char stream1[4096];
    char stream2[4096];
    char stream3[4096];

    WriteToBuffer(stream1, "stream1");
    WriteToBuffer(stream2, "stream2");
    WriteToBuffer(stream3, "stream233333333333333333333");

    {
        printf("* streamId doesn't exist\n");
        assert(!index->Lookup(stream1));
    }

    {
        printf("* insert streamId\n");
        index->Insert(stream1);
        assert(index->Lookup(stream1));
    }

    {
        printf("* insert existing\n");
        index->Insert(stream1);
        assert(index->Lookup(stream1));
    }

    {
        printf("* insert another streamId\n");
        index->Insert(stream2);
        assert(index->Lookup(stream1));
        assert(index->Lookup(stream2));
        assert(!index->Lookup(stream3));
    }

    printf("TestEventStreamIndex... Done!\n");
}

#include "Indexes/PositionIndex.hpp"

void TestPositionIndex()
{
    printf("TestPositionIndex\n");
    auto random = new Random();
    auto allocator = new InMemoryAllocator();
    auto pool = new MemoryPool(allocator);
    auto index = new PositionIndex(pool);

    position_t position1;
    position_t position2;
    position_t position3;

    random->Scramble(position1, sizeof(position_t));
    random->Scramble(position2, sizeof(position_t));
    random->Scramble(position3, sizeof(position_t));

    {
        printf("* insert position\n");
        index->Insert(position1);
        assert(index->Lookup(position1));
        assert(!index->Lookup(position2));
    }

    {
        printf("* scramble\n");
        random->Scramble(position1, sizeof(position_t));
        assert(!index->Lookup(position1));
    }

    {
        printf("* print\n");
        index->Insert(position2);
        index->Insert(position3);
    }

    printf("TestPositionIndex... Done!\n");
}

int CreateEvent(char *buffer, int len, eventid_t eventId, const char *streamId, int version, const char *metadata, const char *payload, const char *eventType)
{
    int length =
        // 0                                 //
        // + sizeof(int)                     // Length
        // + sizeof(version)                 // Version
        // + sizeof(long)                 // Position
        // + sizeof(eventid_t)               // EventId
        // + sizeof(int)                     //StreamIdOffset
        // + sizeof(int)                     //MetadataOffset
        // + sizeof(int)                     //PayloadOffset
        sizeof(Event)
        + strlen(eventType) + sizeof(int) //
        + strlen(streamId) + sizeof(int)  //
        + strlen(metadata) + sizeof(int)  //
        + strlen(payload) + sizeof(int)   //
        //+ sizeof(sha256_t)
        ;

    if (length > len)
    {
        return -1;
    }

    auto event = (Event *)buffer;

    event->Length = length;
    event->Version = version;
    event->Position = 0;
    memcpy(event->EventId, eventId, sizeof(eventid_t));

    char *addr = event->Data;
    int offset = 0;

    // EventType
    *(int *)&addr[offset] = strlen(eventType);
    offset += 4;
    memcpy(&addr[offset], eventType, strlen(eventType));
    offset += strlen(eventType);

    // StreamId
    event->StreamIdOffset = offset;
    *(int *)&addr[offset] = strlen(streamId);
    //assert(event->StreamId() == &addr[offset]);
    offset += 4;
    memcpy(&addr[offset], streamId, strlen(streamId));
    offset += strlen(streamId);

    // Metadata
    event->MetadataOffset = offset;
    *(int *)&addr[offset] = strlen(metadata);
    offset += 4;
    memcpy(&addr[offset], metadata, strlen(metadata));
    offset += strlen(metadata);

    // Payload
    event->PayloadOffset = offset;
    *(int *)&addr[offset] = strlen(payload);
    offset += 4;
    memcpy(&addr[offset], payload, strlen(payload));
    offset += strlen(payload);
    //bzero(&addr[offset], sizeof(sha256_t));


    return length;
}

// struct Event
// {
//   int Length;
//   int Version;
//   eventid_t EventId;
//   char Data[];
// };