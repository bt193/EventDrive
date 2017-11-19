#include "btree.h"
#include <stdlib.h>
#include <strings.h>

void *Lookup(BTreeNode *node, void *key, int (*compar)(const void *, const void *))
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

void Insert(BTreeNode **node, void *key, void *value, int (*compar)(const void *, const void *))
{
    if (*node == NULL)
    {
        BTreeNode *new = malloc(sizeof(BTreeNode));

        new->key = key;
        new->value = value;
        new->left = NULL;
        new->right = NULL;
        *node = new;
        return;
    }

    int result = (*compar)(key, (*node)->key);

    if (result == 0)
    {
        (*node)->value = value;
    }
    else if (result < 1)
    {
        Insert(&(*node)->left, key, value, compar);
    }
    else
    {
        Insert(&(*node)->right, key, value, compar);
    }
}