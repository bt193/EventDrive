#include "btree.h"
#include <stdlib.h>
#include <strings.h>

void *Lookup(struct BTreeNode *node, void *key, int (*compar) (const void *, const void *))
{
    while (node && node->key)
    {
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
    return NULL;
}

void Insert(struct BTreeNode *node, void *key, void *value, int (*compar) (const void *, const void *))
{
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