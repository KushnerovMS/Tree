#include <stdio.h>
#include <new>
#include <assert.h>

#include "lib/Logs.h"

#include "Tree.h"

using namespace Tree;

int Tree::defaultCmp (const void* a, const void* b)
{
    return *((const int*) a) - *((const int*) b);
}

Root::Root (Node* rootNode, size_t dataSize, int (*comparator) (const void* a, const void* b)):
    rootNode_ (rootNode),
    size_ (1),
    dataSise_ (dataSize),
    cmp_ (comparator)
{
    if (rootNode == nullptr)
    {
        Logs.error ("Root crt: RootNode has null pointer", __func__);
        abort ();
    }

    if (cmp_ == nullptr)
    {
        Logs.warn ("Root ctr: comparator has null pointer\n"
                   "Data will be compared as integers");
        cmp_ = defaultCmp;
    }

    if (this == nullptr)
    {
        Logs.error ("Root was not constructed");
        abort ();
    }
}

Root::~Root ()
{
    rootNode_ = nullptr;
}

Node* Root::getRootNode ()
{
    return rootNode_;
}

size_t Root::getSize ()
{
    return size_;
}

size_t Root::incrSize ()
{
    return size_ ++;
}

size_t Root::decrSize ()
{
    if (size_ == 0)
    {
        Logs.warn ("Root::decrSize: Prevented attemp to decrease 0 size");
        return size_;
    }
    return size_ --;
}

size_t Root::getDataSize ()
{
    return dataSise_;
}

int Root::cmp (const void* a, const void* b)
{
    return cmp_ (a, b);
}
