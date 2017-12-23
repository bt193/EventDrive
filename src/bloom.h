#ifndef BLOOM_H
#define BLOOM_H

typedef char bool;

struct BloomItem
{
    void *key;
    void *value;
};

struct BloomContext
{
    int size;
    struct BloomItem *items;
};

typedef struct BloomContext BloomContext;
typedef struct BloomItem BloomItem;

void bloom_init(BloomContext *context, int size, void *key);
void *bloom_lookup(BloomContext *context, void *key);
void bloom_insert(BloomContext *context, void *key, void *value);

#endif
