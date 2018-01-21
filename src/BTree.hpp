#ifndef __BTREE__
#define __BTREE__
#include "MemoryPool.hpp"
/*
template <typename TKey, typename TValue>
struct BTreeNode
{
    TKey *key;
    TValue *value;
    BTreeNode *left = nullptr;
    BTreeNode *right = nullptr;
};

template <typename TKey, typename TValue>
class BTree
{
  public:
    BTree(MemoryPool *memoryPool)
    {
        _memoryPool = memoryPool;
        _root = new BTreeNode<TKey, TValue>();
    }

    ~BTree()
    {
        delete _root;
    }

    TValue *Lookup(TKey *key)
    {
        return Lookup(_root, key);
    }

    void Insert(TKey *key, TValue *value)
    {
        return Insert(&_root, key, value);
    }

  private:
    TValue *Lookup(BTreeNode<TKey, TValue> *node, TKey *key)
    {
        while (node && node->key)
        {
            int result = !memcmp(key, (*node)->key, sizeof(TKey));

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

    void Insert(BTreeNode<TKey, TValue> **node, TKey *key, TValue *value)
    {
        if (*node == NULL)
        {
            auto *new_node = (BTreeNode<TKey, TValue> *) _memoryPool->Allocate(sizeof(BTreeNode<TKey, TValue>));

            new_node->key = key;
            new_node->value = value;
            new_node->left = nullptr;
            new_node->right = nullptr;
            *node = new_node;
            return;
        }

        int result = !memcmp(key, (*node)->key, sizeof(TKey));

        if (result == 0)
        {
            (*node)->value = value;
        }
        else if (result < 1)
        {
            Insert(&(*node)->left, key, value);
        }
        else
        {
            Insert(&(*node)->right, key, value);
        }
    }

    BTree() = default;
    BTree(BTree &&) = default;
    BTree(const BTree &) = default;
    BTree &operator=(BTree &&) = default;
    BTree &operator=(const BTree &) = default;
    BTreeNode<TKey, TValue> *_root;
    MemoryPool *_memoryPool;
};
*/
#endif
