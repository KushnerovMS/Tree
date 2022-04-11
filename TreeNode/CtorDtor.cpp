#include <string.h>
#include <new>
#include <assert.h>

#include "../lib/Logs.h"

#include "../Tree.h"

using namespace Tree;

Node::Node (void* data, size_t dataSize, int (*cmp) (const void* a, const void* b)):
    data_ (nullptr),

    left_ (nullptr),
    right_ (nullptr),

    root_ (new Root (this, dataSize, cmp))
{
    if (data == nullptr)
        Logs.warn ("Tree::Node crt:: Data has null pointer");
    else
    {
        if (dataSize > 0)
        {
            data_ = new char [dataSize];
            memcpy (data_, data, dataSize);
        }
        else
            data_ = data;
    }

    if (root_ == nullptr)
    {
        Logs.error ("Node crt: Tree root can not be created");
        abort ();
    }

    if (this == nullptr)
    {
        Logs.error ("Node was not constructed");
        abort ();
    }
}

Node::Node (Root* root, void* data):
    data_ (nullptr),

    left_ (nullptr),
    right_ (nullptr),

    root_ (root)
{
    assert (root);

    if (data == nullptr)
        Logs.warn ("Node crt: Data_ has null pointer");
    else
    {
        if (root -> getDataSize () > 0)
        {
            data_ = new char [root_ -> getDataSize ()];
            memcpy (data_, data, root_ -> getDataSize ());
        }
        else
        
            data_ = data;
    }

    if (this == nullptr)
    {
        Logs.error ("Node was not constructed");
        abort ();
    }

    root -> incrSize ();
}


Node::~Node ()
{
    if (left_ != nullptr)
        delete left_;

    if (right_ != nullptr)
        delete right_;

    if (data_ != nullptr && root_ -> getDataSize () >= 0)
        delete[] (char*)data_;
    data_ = nullptr;

    left_ = nullptr;
    right_ = nullptr;

    root_ -> decrSize ();

    if (root_ -> getRootNode () == this)
        delete root_;

    root_ = nullptr;
}
