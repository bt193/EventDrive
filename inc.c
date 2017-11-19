#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "sha1.h"
#include "btree.h"

typedef char byte_t;
typedef byte_t guid_t[16];
typedef byte_t sha1_t[20];
typedef guid_t position_t;
typedef char bool;

#define true 1
#define false 0

static int counter = 0;
const int Event = 1;

typedef struct
{
    uint32_t length;
    uint32_t op;
    sha1_t hash;
    char payload[];
} FragmentHeader;

typedef struct
{
    uint32_t length;
    uint32_t op;
    sha1_t hash;
    guid_t eventId;
} FragmentEvent;

struct FragmentNode
{
    position_t position;
    FragmentHeader *event;
    struct FragmentNode *next;
    struct FragmentNode *nextInStream;
};

static struct FragmentNode first, *last = &first;
static SHA1_CTX sha1_ctx;
static position_t empty_position;

static struct BTreeNode positionTree;

int ComparePosition(const void *a, const void *b)
{
    return memcmp(a, b, sizeof(position_t));
}

extern void Initialize()
{
    sha1_init(&sha1_ctx);
    Insert(&positionTree, empty_position, &first, ComparePosition);
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

    struct FragmentNode *node = (struct FragmentNode *)malloc(sizeof(struct FragmentNode));

    node->event = fragment;
    memcpy(node->position, ((FragmentEvent *) fragment)->eventId, sizeof(position_t));
    last->next = node;
    last = node;

    Insert(&positionTree, node->position, node, ComparePosition);
}

extern int ReadEventsFromFrom(position_t position, char buffer[], int length)
{
    struct FragmentNode *iter, *last;

    if (!memcmp(position, empty_position, sizeof(position_t)))
    {
        iter = last = &first;
    }
    else
    {
        iter = last = (struct FragmentNode *) Lookup(&positionTree, position, ComparePosition);
    }

    if (iter == NULL)
    {
        return -1;
    }

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

    memcpy(position, &last->position, sizeof(position_t));
    return len;
}