#include "MemorySegment.hpp"
#include <string.h>

MemorySegment::MemorySegment(char *memory, int length, int fullLength)
{
    _memory = memory;
    _length = length;
    _fullLength = fullLength;
}

MemorySegment::~MemorySegment()
{
}

void MemorySegment::FlushAsync()
{
}

void MemorySegment::FlushSync()
{
}

bool MemorySegment::BeginTransaction()
{
    int buffer[]{OpCode::BeginTransaction};
    auto dest = Peek(sizeof(buffer));

    return Put((char *)buffer, sizeof(buffer));
}

bool MemorySegment::Commit()
{
    int buffer[]{OpCode::Commit};
    auto dest = Peek(sizeof(buffer));

    return Put((char *)buffer, sizeof(buffer));
}

bool MemorySegment::Put(char *memory, int length)
{
    auto dest = Peek(length);

    if (!dest)
    {
        return false;
    }

    memcpy(dest, memory, length);
    _offset += length;
    _dirty = true;
    return true;
}

char *MemorySegment::Peek(int length)
{
    if (_offset + length > _length)
    {
        return nullptr;
    }

    return _memory + _offset;
}
