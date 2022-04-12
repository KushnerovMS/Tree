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
    if (dataSize > 0)
    {
        data_ = new char [dataSize];

        if (data)
            memcpy (data_, data, dataSize);
        else
            memset (data_, 0, dataSize);
            
    }
    else
        data_ = data;

    if (root_ == nullptr)
    {
        Logs.error ("Node crt: Tree root can not be created");
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

    if (root -> getDataSize () > 0)
    {
        data_ = new char [root_ -> getDataSize ()];
        if (data)
            memcpy (data_, data, root_ -> getDataSize ());
        else
            memset (data_, 0, root_ -> getDataSize());
    }
    else
        data_ = data;

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
