#include <stdio.h>
#include <string.h>
#include <new>
#include <assert.h>

#include "lib/Logs.h"

#include "Tree.h"

using namespace Tree;

Node::Node (void* data, size_t dataSize, bool destructDataWhileDestructing, int (*cmp) (const void* a, const void* b)):
    data_ (nullptr),

    left_ (nullptr),
    right_ (nullptr),

    root_ (new Root (this, dataSize, destructDataWhileDestructing, cmp))
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
    data_ (),

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

    Logs.trace ("Dtor: Data = %p", data_);
    if (data_ != nullptr && (root_ -> getDataSize () != 0 || root_ -> isDestructDataWhileDestructing ()))
        delete [] (data_);
    data_ = nullptr;

    left_ = nullptr;
    right_ = nullptr;

    root_ -> decrSize ();

    if (root_ -> getRootNode () == this)
        delete root_;

    root_ = nullptr;
}

Root* Node::getRoot ()
{
    return root_;
}

Node* Node::addOnLeft (Node* node)
{
    assert (node);

    if (left_ != nullptr)
    {
        Logs.error ("Node::AddOnLeft: There is already tree on the left with pointer %p", left_);
        abort ();
    }

    if (node -> getRoot () != root_)
    {
        Logs.error ("Node::AddonLeft: Nodes has different roots");
        return nullptr;
    }

    left_ = node;
    
    return node;
}

Node* Node::addOnRight (Node* node)
{
    assert (node);

    if (right_ != nullptr)
    {
        Logs.error ("Node::Addonright: There is already tree on the right with pointer %p", right_);
        abort ();
    }

    if (node -> getRoot () != root_)
    {
        Logs.error ("Node::AddonRight: Nodes has different roots");
        return nullptr;
    }

    right_ = node;
    
    return node;
}


Node* Node::getLeftNode ()
{
    return left_;
}

Node* Node::getRightNode()
{
    return right_;
}


void Node::print (FILE* file, PRINT_MODE mode, bool compact, void (*dataDump) (FILE* file, const void* data), const char* beginSep, const char* endSep, size_t level)
{
    assert (file);
    assert (dataDump);

    if (left_ == 0 && right_ == 0) 
    {
        fprintf (file, "%*s%s", level * 4 * (! compact), "", beginSep);

        dataDump (file, data_);

        fprintf (file, "%s", endSep);
        if (! compact) fputc ('\n', file);
    }

    else
    {
        fprintf (file, "%*s%s", level * 4 * (! compact), "", beginSep);
        if (mode == PRINT_MODE::PRE_ORDER) dataDump (file, data_);
        if (! compact) fputc ('\n', file);

        if (left_)
            left_ -> print (file, mode, compact, dataDump, beginSep, endSep, level + 1); 
        else
        {
            fprintf (file, "%*s%s%s", (level + 1) * 4 * (! compact), "", beginSep, endSep);
            if (! compact) fputc ('\n', file);
        }

        if (mode == PRINT_MODE::IN_ORDER)
        {
            fprintf (file, "%*s", level * 4 * (! compact), "");
            dataDump (file, data_);
            if (! compact) fputc ('\n', file);
        }

        if (right_)
            right_ -> print (file, mode, compact, dataDump, beginSep, endSep, level + 1); 
        else
        {
            fprintf (file, "%*s%s%s", (level + 1) * 4 * (! compact), "", beginSep, endSep);
            if (! compact) fputc ('\n', file);
        }

        fprintf (file, "%*s", level * 4 * (! compact), "");
        if (mode == PRINT_MODE::POST_ORDER) dataDump (file, data_);

        fprintf (file, "%s", endSep);
        if (! compact || level == 0) fputc ('\n', file);
    }
}

void Tree::defaultDataDump (FILE* file, const void* data)
{
    if (data)
        fprintf (file, "%d", *((const int*) data));
    else
        fprintf (file, "");
}


void* Node::getData ()
{
    return data_;
}

void Node::setData (void* data)
{

    if (root_ -> getDataSize () != 0)
    {
        if (data != nullptr)
            memcpy (data_, data, root_ -> getDataSize ());
        else
        {
            memset (data_, 0, root_ -> getDataSize ());
        }
    }
    else
        data_ = data;
}


void* Node::search (void* data, bool create)
{
    Node* node = this;

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
                    return node -> addOnLeft (new Node (root_, data)) -> getData ();
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
                    return node -> addOnRight (new Node (root_, data)) -> getData ();
                else
                    return nullptr;
            }
        }

        else
        {
            return node -> getData ();
        }
    }
}
