#include <stdio.h>
#include <new>
#include <assert.h>

#include "lib/Logs.h"

#include "Tree.h"

int Tree::defaultCmp (const void* a, const void* b)
{
    return *((const int*) a) - *((const int*) b);
}

Tree::Root::Root (Tree::Node* rootNode, int (*cmp) (const void* a, const void* b)):
    rootNode_ (rootNode),
    size_ (1),
    cmp_ (cmp)
{
    if (rootNode == nullptr)
    {
        Logs.error ("Tree::Root crt: RootNode has null pointer", __func__);
        abort ();
    }

    if (cmp == nullptr)
    {
        Logs.warn ("Tree::Root ctr: comparator has null pointer\n"
                   "Data will be compared as integers");
        cmp_ = defaultCmp;
    }

    if (this == nullptr)
    {
        Logs.error ("Tree::Root was not constructed");
        abort ();
    }
}

Tree::Root::~Root ()
{
    rootNode_ = nullptr;
}

Tree::Node* Tree::Root::getRootNode ()
{
    return rootNode_;
}

size_t Tree::Root::getSize ()
{
    return size_;
}

size_t Tree::Root::incrSize ()
{
    return size_ ++;
}

size_t Tree::Root::decrSize ()
{
    if (size_ == 0)
    {
        Logs.warn ("Tree::Root::decrSize: Prevented attemp to decrease 0 size");
        return size_;
    }
    return size_ --;
}

