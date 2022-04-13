#include <stdio.h>
#include <new>
#include <assert.h>

#include <kms/Logs.h>

#include "Tree.h"

using namespace Tree;

int Tree::defaultCmp (const void* a, const void* b)
{
    return *((const int*) a) - *((const int*) b);
}

Root::Root (Node* rootNode,
            size_t dataSize,
            int (*comparator) (const void* a, const void* b)
            ):
    rootNode_ (rootNode),
    size_ (1),
    cmp_ (comparator),
    dataSize_ (dataSize)
{
    assert (comparator);
    assert (rootNode);

    if (this == nullptr)
    {
        Logs.error ("Root was not constructed");
        abort ();
    }
}

Root::~Root ()
{
    rootNode_ = nullptr;
    size_ = 0;
    dataSize_ = 0;
    cmp_ = nullptr;
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
    return dataSize_;
}


int Root::cmp (const void* a, const void* b)
{
    return cmp_ (a, b);
}
