#ifndef BLOOM_H
#define BLOOM_H

typedef char bool;

struct BloomItem
{
    void *key;
    void *value;
    bool real;
};

struct BloomContext
{
    int size;
    struct BloomItem items;
};

void bloom_init(struct BloomContext *context, int size);
void *bloom_lookup(struct BloomContext *context, void *key);
void bloom_insert(struct BloomContext *context, void *key, void *value);

#endif
