#include "EventStore.hpp"

#include <stdio.h>
#include <string.h>

#include "BTree.hpp"

void TestEventStreamIndex();
void TestEventCollisionIndex();
void TestPositionIndex();

int main(int argc, char *argv[])
{
    TestEventCollisionIndex();
    TestEventStreamIndex();
    TestPositionIndex();
    // char buffer[1000];

    // bzero(buffer, sizeof(buffer));

    // EventStore eventStore;

    // eventStore.Initialize();

    // for (auto i = 0; i < 1; ++i)
    // {
    //     eventStore.Put(buffer, sizeof(buffer));
    // }

    //scanf(buffer);
    return 0;
}

#include "Indexes/EventCollisionIndex.hpp"
#include "Allocators/InMemoryAllocator.hpp"
#include "Random.hpp"
#include <assert.h>

void TestEventCollisionIndex()
{
    printf("TestEventCollisionIndex\n");
    auto random = new Random();
    auto allocator = new InMemoryAllocator();
    auto pool = new MemoryPool(allocator);
    auto index = new EventCollisionIndex(pool);
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
        index->Insert(id1);

        assert(index->Exists(id1));
    }

    {
        printf("* insert existing\n");
        index->Insert(id1);
    }

    {
        printf("* insert another\n");
        index->Insert(id2);

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

void WriteToBuffer(char *buffer, char *text)
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