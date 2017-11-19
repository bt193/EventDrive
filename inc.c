#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "sha1.h"
#include "btree.h"
#include "bloom.h"

typedef char byte_t;
typedef byte_t guid_t[16];
typedef byte_t sha1_t[20];
typedef guid_t position_t;
typedef char bool;

#define true 1
#define false 0

const int Event = 1;

struct FragmentHeader
{
    uint32_t length;
    uint32_t op;
    sha1_t hash;
    char payload[];
};

struct FragmentEvent
{
    uint32_t length;
    uint32_t op;
    sha1_t hash;
    guid_t eventId;
};

struct FragmentNode
{
    position_t position;
    struct FragmentHeader *event;
    struct FragmentNode *next;
    struct FragmentNode *nextInStream;
};

typedef struct FragmentEvent FragmentEvent;
typedef struct FragmentNode FragmentNode;
typedef struct FragmentHeader FragmentHeader;

static FragmentNode *first, *last;
static SHA1_CTX sha1_ctx;
static position_t empty_position;
static BloomContext bloom;
static BTreeNode *positionBtree;

int ComparePosition(const void *a, const void *b)
{
    return memcmp(a, b, sizeof(position_t));
}

extern void Initialize()
{
    first = last = (FragmentNode *)malloc(sizeof(FragmentNode));
    bzero(first, sizeof(FragmentNode));
    sha1_init(&sha1_ctx);
    Insert(&positionBtree, empty_position, &first, ComparePosition);
    bloom_init(&bloom, 512, empty_position);
}

extern void AppendEvent(char event[], int length)
{
    int len = length + sizeof(FragmentHeader);
    FragmentHeader *fragment = (FragmentHeader *)malloc(len);

    sha1_update(&sha1_ctx, (char *)&len, sizeof(len));
    sha1_update(&sha1_ctx, event, length);

    fragment->length = len;
    fragment->op = Event;
    sha1_final(&sha1_ctx, fragment->hash);
    memcpy(fragment->payload, event, length);

    FragmentNode *node = (FragmentNode *)malloc(sizeof(FragmentNode));
    node->event = fragment;
    node->next = NULL;
    memcpy(node->position, ((FragmentEvent *)fragment)->eventId, sizeof(position_t));
    last->next = node;
    last->nextInStream = NULL;
    last = node;

    Insert(&positionBtree, node->position, node, ComparePosition);
}

extern int ReadEventsFrom(position_t position, char buffer[], int length)
{
    FragmentNode *iter, *last;

    if (!memcmp(position, empty_position, sizeof(position_t)))
    {
        iter = first;
    }
    else
    {
        iter = (FragmentNode *)bloom_lookup(&bloom, position);

        if (!iter)
        {
            iter = (FragmentNode *)Lookup(positionBtree, position, ComparePosition);
        }
    }

    if (iter == NULL)
    {
        return -1;
    }

    last = iter;

    int len = 0;

    while (iter = iter->next)
    {
        if (len + iter->event->length >= length)
        {
            break;
        }

        memcpy(&buffer[len], iter->event, iter->event->length);
        len += iter->event->length;
        last = iter;
    }

    memcpy(position, last->position, sizeof(position_t));
    bloom_insert(&bloom, last->position, last);
    return len;
}
