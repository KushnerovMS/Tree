#include <string.h>

#include <kms/Logs.h>

#include "../Tree.h"

using namespace Tree;

Root* Node::getRoot ()
{
    return root_;
}

Node* Node::setLeftNode (Node* node)
{
    if (node != nullptr && node -> getRoot () != root_)
    {
        Logs.error ("Node::SetRightNode: Nodes with pointers %p & %p  has different roots", this, node);
        return nullptr;
    }

    left_ = node;
    
    return node;
}

Node* Node::setRightNode (Node* node)
{
    if (node != nullptr && node -> getRoot () != root_)
    {
        Logs.error ("Node::SetRightNode: Nodes with pointers %p & %p  has different roots", this, node);
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


void* Node::getData ()
{
    return data_;
}

void Node::setData (void* data)
{
    if (root_ -> getDataSize() > 0)
    {
        if (data != nullptr)
            memcpy (data_, data, root_ -> getDataSize());
        else
            memset (data_, 0, root_ -> getDataSize());
    }
    else
        data_ = data;
}
