#ifndef BTREE_H
#define BTREE_H

struct BTreeNode
{
    void *key;
    void *value;
    struct BTreeNode *left;
    struct BTreeNode *right;
};

typedef struct BTreeNode BTreeNode;

void *Lookup(BTreeNode *node, void *key, int (*compar)(const void *, const void *));
void Insert(BTreeNode **node, void *key, void *value, int (*compar)(const void *, const void *));

#endif
