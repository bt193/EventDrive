#include "bloom.h"
#include <stdlib.h>
#include <string.h>

/* FUSK */
typedef char position_t[16];

int hash(char key[]);

void bloom_init(BloomContext *context, int size, void *key, void *value)
{
    context->size = size;
    context->items = malloc(sizeof(BloomItem) * size);

    for (int i = 0; i < size; ++i)
    {
        context->items[i].key = key;
        context->items[i].value = value;
    }
}

void *bloom_lookup(BloomContext *context, void *key)
{
    int index = hash(key) % context->size;

    if (!memcmp(key, context->items[index].key, sizeof(position_t)))
    {
        return context->items[index].value;
    }

    return 0;
}

void bloom_insert(BloomContext *context, void *key, void *value)
{
    int index = hash(key) % context->size;

    context->items[index].key = key;
    context->items[index].value = value;
}

int hash(char key[])
{
    int x = 0xFFFFFFFF;

    for (int i = 0; i < sizeof(position_t); ++i)
    {
        char ch = key[i];

        x = (x ^ ch) << 4 + ch;
    }
    return x & 0x7FFFFFFF;
}
