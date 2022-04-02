#include <stdio.h>
#include <string.h>
#include <new>
#include <assert.h>

#include "lib/Logs.h"

#include "Tree.h"

using namespace Tree;

Node::Node (void* data, size_t dataSize, int (*cmp) (const void* a, const void* b)):
    data_ (new char [dataSize]),

    left_ (nullptr),
    right_ (nullptr),

    root_ (new Root (this, dataSize, cmp))
{
    if (data == nullptr)
        Logs.warn ("Node crt:: Data_ has null pointer");
    else
        memcpy (data_, data, dataSize);

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
    data_ (new char [root -> getDataSize ()]),

    left_ (nullptr),
    right_ (nullptr),

    root_ (root)
{
    if (data == nullptr)
        Logs.warn ("Node crt: Data_ has null pointer");
    else
        memcpy (data_, data, root_ -> getDataSize ());

    if (root_ == nullptr)
    {
        Logs.error ("Node crt: Tree root has null pointer");
        abort ();
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
    if (data_ != nullptr)
        delete [] ((char*) data_);
    data_ = nullptr;

    left_ = nullptr;
    right_ = nullptr;

    root_ -> decrSize ();

    if (root_ -> getRootNode () == this)
        delete root_;

    root_ = nullptr;
}


Node* Node::addOnLeft (void* data)
{
    if (data == nullptr)
        Logs.warn ("Node::addOnLeft: Data has null pointer");

    if (left_ != nullptr)
    {
        Logs.error ("Node::AddonLeft: There is already tree on the left with pointer %p", left_);
        abort ();
    }
    
    left_ = new Node (root_, data);

    if (left_ == nullptr)
    {
        Logs.error ("Node::addOnLeft: Error of creating");
        abort ();
    }
    
    root_ -> incrSize ();

    return left_;
}

Node* Node::addOnRight (void* data)
{
    if (data == nullptr)
        Logs.warn ("Node::addOnRight: Data has null pointer");

    if (right_ != nullptr)
    {
        Logs.error ("Node::AddonRight: There is already tree on the right with pointer %p", left_);
        abort ();
    }
    
    right_ = new Node (root_, data);

    if (right_ == nullptr)
    {
        Logs.error ("Node::addOnRight: Error of creating");
        abort ();
    }

    root_ -> incrSize ();

    return right_;
}


Node* Node::getLeftNode ()
{
    return left_;
}

Node* Node::getRightNode()
{
    return right_;
}


void Node::dump (FILE* file, void (*dataDump) (const void* data, FILE* file), size_t level)
{
    assert (file);

    if (dataDump == nullptr)
        dataDump = defaultDataDump;

    fprintf (file, "%*s{", level * 4, "");
    

    if (left_ != nullptr)
    {
        fputc ('\n', file);
        left_ -> dump (file, dataDump, level + 1); 
        fputc ('\n', file);
    }
    else if (right_ != nullptr)
        fputc ('\n', file);


    if (left_ != nullptr || right_ != nullptr)
        fprintf (file, "%*s", level * 4, "");

    dataDump (data_, file);


    if (right_ != nullptr)
    {
        fputc ('\n', file);
        right_ -> dump (file, dataDump, level + 1); 
        fprintf (file, "\n%*s", level * 4, "");
    }
    else if (left_ != nullptr)
        fprintf (file, "\n%*s", level * 4, "");


    fprintf (file, "}");
}

void Tree::defaultDataDump (const void* data, FILE* file)
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
    assert (data);

    memcpy (data_, data, root_ -> getDataSize ());
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
