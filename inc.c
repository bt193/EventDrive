#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cuda_stdint.h>

#include "sha1.h"
#include "btree.h"

typedef char byte_t;
typedef byte_t guid_t[16];
typedef byte_t guid_t[16];
typedef byte_t sha1_t[20];
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

struct FragmentNode
{
    sha1_t position;
    FragmentHeader *event;
    struct FragmentNode *next;
    struct FragmentNode *nextInStream;
};

static struct FragmentNode *first, *last;
static SHA1_CTX sha1_ctx;
static sha1_t empty_hash;

//static struct BTreeNode eventIdTree;
static struct BTreeNode positionTree;

// int CompareHash(const void *a, const void *b)
// {
//     return memcmp(a, b, sizeof(guid_t));
// }

void PrintHex(char *head, char *buffer, int length)
{
    return;
    printf(head);
    for (int i = 0; i < length; ++i)
    {
        printf("%02hhx", buffer[i]);
    }
    printf("\n");
}

int ComparePosition(const void *a, const void *b)
{
    return memcmp(a, b, sizeof(sha1_t));
}

extern void Initialize()
{
    first = last = (struct FragmentNode *)malloc(sizeof(struct FragmentNode));
    bzero(first, sizeof(struct FragmentNode));
    sha1_init(&sha1_ctx);

    Insert(&positionTree, empty_hash, first, ComparePosition);
}

extern void AppendEvent(char *event, int length)
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
    sha1_final(&sha1_ctx, node->position);
    last->next = node;
    last = node;

    Insert(&positionTree, node->position, node, ComparePosition);
    //printf("Lookup: %p\n", Lookup(&positionTree, node->position, ComparePositionIns));
}

struct FragmentNode *FindPosition(char *position)
{
    return (struct FragmentNode *) Lookup(&positionTree, position, ComparePosition);
}

// void Walk(struct BTreeNode *node, int depth)
// {
//     if (!node)
//     {
//         return;
//     }
//     PrintHex("Walk: ", node->key, sizeof(sha1_t));
//     Walk(node->left, depth + 1);
//     Walk(node->right, depth + 1);
// }

extern int ReadEventsFromFrom(char *position, char *buffer, int length)
{
    // PrintHex("before walk: ", positionTree.key, sizeof(sha1_t));
    // printf("walk - begin\n");
    // Walk(&positionTree, 0);
    // printf("walk - end\n");


    struct FragmentNode *iter = (struct FragmentNode *)FindPosition(position);
    struct FragmentNode *last = iter;
    int len = 0;

    if (iter == NULL)
    {
        return -1;
    }

    while (iter = iter->next)
    {
        if (len + iter->event->length >= length)
        {
            break;
        }

        last = iter;
        memcpy(&buffer[len], iter->event, iter->event->length);
        len += iter->event->length;
    }
    memcpy(position, &last->position, sizeof(sha1_t));
    return len;
}