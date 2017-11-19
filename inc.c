#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cuda_stdint.h>

#include "sha1.h"

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

struct BTreeNode
{
    void *key;
    void *value;
    struct BTreeNode *left;
    struct BTreeNode *right;
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

void *Lookup(struct BTreeNode *node, void *key, int (*compar) (const void *, const void *))
{
    //printf("start - Lookup\n");
    while (node && node->key)
    {
        //PrintHex("", node->key, sizeof(sha1_t));
        int result = (*compar) (key, node->key);

        if (result == 0)
        {
            return node->value;
        }
        else if (result < 0)
        {
            node = node->left;
        }
        else
        {
            node = node->right;
        }
    }
    //printf("end - Lookup\n");
    return NULL;
}

void Insert(struct BTreeNode *node, void *key, void *value, int (*compar) (const void *, const void *))
{
    //PrintHex("Insert: ", key, sizeof(sha1_t));
    if (!node->key)
    {
        node->key = key;
        node->value = value;
        return;
    }

    while (node)
    {
        int result = (*compar) (key, node->key);

        if (result == 0)
        {
            node->value = value;
            return;
        }
        else if (result < 0)
        {
            if (node->left == NULL)
            {
                struct BTreeNode *new = malloc(sizeof(struct BTreeNode));

                bzero(new, sizeof(struct BTreeNode));
                new->key = key;
                new->value = value;
                node->left = new;
                return;
            }
            node = node->left;
        }
        else
        {
            if (node->right == NULL)
            {
                struct BTreeNode *new = malloc(sizeof(struct BTreeNode));

                bzero(new, sizeof(struct BTreeNode));
                new->key = key;
                new->value = value;
                node->right = new;
                return;
            }            
            node = node->right;
        }        
    }
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
#if 1
    struct FragmentNode *location = (struct FragmentNode *) Lookup(&positionTree, position, ComparePosition);
#else    
    
    // printf("\nLooking for:\n");
    // PrintHex("", position, sizeof(sha1_t));
    // printf("Lookup: %p\n", Lookup(&positionTree, position, ComparePosition));
    // return location;
    struct FragmentNode *iter = first;
    int index = 0;

    do
    {
        //PrintHex(&iter->position, sizeof(sha1_t));
        if (!memcmp(position, &iter->position, sizeof(sha1_t)))
        {
            return (struct FragmentNode *)iter;
        }
    } while (iter = iter->next);

    return NULL;
#endif    
}

void Walk(struct BTreeNode *node, int depth)
{
    if (!node)
    {
        return;
    }
    PrintHex("Walk: ", node->key, sizeof(sha1_t));
    Walk(node->left, depth + 1);
    Walk(node->right, depth + 1);
}

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