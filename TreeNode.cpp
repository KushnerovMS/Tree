#include <stdio.h>
#include <new>
#include <assert.h>

#include "lib/Logs.h"

#include "Tree.h"


Tree::Node::Node (void* data, int (*cmp) (const void* a, const void* b)):
    data_ (data),

    left_ (nullptr),
    right_ (nullptr),

    root_ (new (std::nothrow) Tree::Root (this, cmp))
{
    if (data == nullptr)
        Logs.warn ("Tree::Node crt:: Data_ has null pointer");

    if (root_ == nullptr)
    {
        Logs.error ("Tree::Node crt: Tree root can not be created");
        abort ();
    }

    if (this == nullptr)
    {
        Logs.error ("Tree::Node was not constructed");
        abort ();
    }
}

Tree::Node::Node (Tree::Root* root, void* data):
    data_ (data),

    left_ (nullptr),
    right_ (nullptr),

    root_ (root)
{
    if (data == nullptr)
        Logs.warn ("Tree::Node crt: Data_ has null pointer");

    if (root_ == nullptr)
    {
        Logs.error ("Tree::Node crt: Tree root has null pointer");
        abort ();
    }

    if (this == nullptr)
    {
        Logs.error ("Tree::Node was not constructed");
        abort ();
    }

    root -> incrSize ();
}

Tree::Node::~Node ()
{
    if (left_ != nullptr)
        delete left_;

    if (right_ != nullptr)
        delete right_;

    if (data_ != nullptr)
        delete data_;
    data_ = nullptr;

    left_ = nullptr;
    right_ = nullptr;

    root_ -> decrSize ();

    if (root_ -> getRootNode () == this)
        delete root_;

    root_ = nullptr;
}


Tree::Node* Tree::Node::addOnLeft (void* data)
{
    if (data == nullptr)
        Logs.warn ("Tree::Node::addOnLeft: Data has null pointer");

    if (left_ != nullptr)
    {
        Logs.error ("Tree::Node::AddonLeft: There is already tree on the left with pointer %p", left_);
        abort ();
    }
    
    left_ = new Tree::Node (root_, data);

    if (left_ == nullptr)
    {
        Logs.error ("Tree::Node::addOnLeft: Error of creating");
        abort ();
    }
    
    root_ -> incrSize ();

    return left_;
}

Tree::Node* Tree::Node::addOnRight (void* data)
{
    if (data == nullptr)
        Logs.warn ("Tree::Node::addOnRight: Data has null pointer");

    if (right_ != nullptr)
    {
        Logs.error ("Tree::Node::AddonRight: There is already tree on the right with pointer %p", left_);
        abort ();
    }
    
    right_ = new Tree::Node (root_, data);

    if (right_ == nullptr)
    {
        Logs.error ("Tree::Node::addOnRight: Error of creating");
        abort ();
    }

    root_ -> incrSize ();

    return right_;
}


Tree::Node* Tree::Node::getLeftNode ()
{
    return left_;
}

Tree::Node* Tree::Node::getRightNode()
{
    return right_;
}


Tree::Error Tree::Node::dump (FILE* file, int (*dataDump) (const void* data, FILE* file))
{
    assert (file);

    Tree::Error err = NO_ERRORS;

    fputc ('{', file);

    if (dataDump == nullptr)
        dataDump = defaultDataDump;

    if (dataDump (data_, file) != 0)
        err = BAD_DATA;

    Tree::Error childErr = NO_ERRORS;

    if (left_)
    {
        left_ -> dump (file, dataDump);
        if (childErr != NO_ERRORS)
            err = childErr;
    }
    if (right_)
    {
        right_ -> dump (file, dataDump);
        if (childErr != NO_ERRORS)
            err = childErr;
    }

    fputc ('}', file);

    return err;
}

int Tree::defaultDataDump (const void* data, FILE* file)
{
    if (data)
    {
        fprintf (file, "%d", *((const int*) data));
        return 0;
    }
    else
    {
        fprintf (file, "(null)");
        return 1;
    }
}


void* Tree::Node::getData ()
{
    return data_;
}

void Tree::Node::setData (void* data)
{
    data_ = data;
    Logs.warn ("Tree::Node::setData: Data has null pointer");
}


void* Tree::Node::search (void* data, bool create)
{
    Tree::Node* node = this;

    int res = 0;

    for (size_t i = 0; i < root_ -> getSize (); i ++)
    {
        res = root_ -> cmp (data, node -> getData ());

        if (res < 0)
        {
            if (node -> getLeftNode () != nullptr)
            {
                node = node -> getLeftNode ();
            }
            else
            {
                if (create)
                    node -> addOnLeft (data);
                else
                    return nullptr;
            }
        }

        else if (res > 0)
        {
            if (node -> getRightNode () != nullptr)
            {
                node = node -> getRightNode ();
            }
            else
            {
                if (create)
                    node -> addOnRight (data);
                else
                    return nullptr;
            }
        }

        else
            return node -> getData ();
    }
}