#include <assert.h>

#include "../lib/Logs.h"

#include "../Tree.h"

using namespace Tree;

Error Node::notOK (size_t counter)
{
    Error err (0);

    static size_t nodeCounter;

    if (counter == 0)
        nodeCounter = 0;

    if (root_ == nullptr)
    {
        err.NULL_ROOT = 1;
        return err;
    }

    if (root_ -> getRootNode() == nullptr)
    {
        err.NULL_ROOT_NODE = 1;
        return err;
    }

    if (root_ -> getSize() < counter)
    {
        err.CICLDED = 1;
        return err;
    }

    if (left_ != 0)
    {
        if (root_ != left_ -> getRoot ())
            err.DIFFERENT_ROOTS;

        err.add (left_ -> notOK(counter + 1));
    }

    if (right_ != 0)
    {
        if (root_ != right_ -> getRoot ())
            err.DIFFERENT_ROOTS;

        err.add (right_ -> notOK(counter + 1));
    }

    nodeCounter ++;

    if (root_ -> getRootNode() == this && root_ -> getSize() != nodeCounter)
        err.ERROR_SIZE = 1;

    return err;
}

Node* Node::search (void* data, bool create)
{
    Node* node = this;

    int res = 0;

    for (size_t i = 0; i < root_ -> getSize (); i ++)
    {
        res = root_ -> cmp (data, node -> getData ());

        if (res < 0)
        {
            if (node -> getLeftNode () != nullptr)
                node = node -> getLeftNode ();
            else
            {
                if (create)
                    return node -> setLeftNode (new Node (root_, data));
                else
                    return nullptr;
            }
        }

        else if (res > 0)
        {
            if (node -> getRightNode () != nullptr)
                node = node -> getRightNode ();
            else
            {
                if (create)
                    return node -> setRightNode (new Node (root_, data));
                else
                    return nullptr;
            }
        }

        else
        {
            return node;
        }
    }
}
