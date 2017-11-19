#ifndef BTREE_H
#define BTREE_H

struct BTreeNode
{
    void *key;
    void *value;
    struct BTreeNode *left;
    struct BTreeNode *right;
};

void *Lookup(struct BTreeNode *node, void *key, int (*compar) (const void *, const void *));
void Insert(struct BTreeNode *node, void *key, void *value, int (*compar) (const void *, const void *));

#endif
