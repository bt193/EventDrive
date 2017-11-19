#include "bloom.h"
#include <stdlib.h>
#include <string.h>

/* FUSK */
typedef char position_t[16];

int hash(char key[]);

void bloom_init(struct BloomContext *context, int size)
{
    context->size = size;
    context->items = malloc(sizeof(struct BloomItem) * size);

    for (int i = 0; i < size; ++i)
    {
        context->items[i].real = 0;
    }
}

void *bloom_lookup(struct BloomContext *context, void *key)
{
    int index = hash(key) % context->size;

    if (context->items[index].real && !memcmp(key, context->items[index].key, sizeof(position_t)))
    {
        return context->items[index].value;
    }

    return 0;
}

void bloom_insert(struct BloomContext *context, void *key, void *value)
{
    int index = hash(key) % context->size;

    context->items[index].key = key;
    context->items[index].value = value;
    context->items[index].real = 1;
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
