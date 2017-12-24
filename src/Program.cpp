#include "EventStore.hpp"

#include <stdio.h>
#include <string.h>

#include "BTree.hpp"

int main(int argc, char *argv[])
{
    char buffer[1000];

    memset(buffer, 0, sizeof(buffer));

    EventStore eventStore;

    eventStore.Initialize();

    for (auto i = 0; i < 5; ++i)
    {
        eventStore.Put(buffer, sizeof(buffer));
    }

    //scanf(buffer);
    return 0;
}
