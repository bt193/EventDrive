#include "EventStore.hpp"

int main(int argc, char *argv[])
{
    char buffer[1000];

    EventStore eventStore;

    for (auto i = 0; i < 1000000; ++i)
    {
        eventStore.Put(buffer, sizeof(buffer));
    }

    return 0;
}