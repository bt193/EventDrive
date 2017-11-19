#include "btree.h"
#include <stdlib.h>
#include <strings.h>

void AllocNode(struct BTreeNode **node, void *key, void *value);

void *Lookup(struct BTreeNode *node, void *key, int (*compar)(const void *, const void *))
{
    while (node && node->key)
    {
        int result = (*compar)(key, node->key);

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
    return NULL;
}

void Insert(struct BTreeNode *node, void *key, void *value, int (*compar)(const void *, const void *))
{
    if (!node->key)
    {
        node->key = key;
        node->value = value;
        return;
    }

    while (node)
    {
        int result = (*compar)(key, node->key);

        if (result == 0)
        {
            node->value = value;
            return;
        }
        else if (result < 0)
        {
            if (node->left == NULL)
            {
                AllocNode(&node->left, key, value);
                return;
            }
            node = node->left;
        }
        else
        {
            if (node->right == NULL)
            {
                AllocNode(&node->right, key, value);
                return;
            }
            node = node->right;
        }
    }
}

void AllocNode(struct BTreeNode **node, void *key, void *value)
{
    struct BTreeNode *new = malloc(sizeof(struct BTreeNode));

    new->key = key;
    new->value = value;
    new->left = NULL;
    new->right = NULL;
    *node = new;
}