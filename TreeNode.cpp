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

    if (data_ != nullptr && (root_ -> getDataSize () >= 0 || root_ -> isDestructDataWhileDestructing ()))
        delete data_;
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

Node* Node::setLeftNode (Node* node)
{
    if (node != nullptr && node -> getRoot () != root_)
    {
        Logs.error ("Node::AddonLeft: Nodes has different roots");
        return nullptr;
    }

    left_ = node;
    
    return node;
}

Node* Node::setRightNode (Node* node)
{
    if (node != nullptr && node -> getRoot () != root_)
    {
        Logs.error ("Node::AddonLeft: Nodes has different roots");
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

#define NEW_LINE    if (! compact) fputc ('\n', file);
#define BEGIN_SEP   fprintf (file, "%s", beginSep);
#define END_SEP     fprintf (file, "%s", endSep);
#define GO_ON_LEVEL(lev) fprintf (file, "%*s", (lev) * 4 * (! compact), "");

    if (left_ == 0 && right_ == 0) 
    {
        GO_ON_LEVEL (level) BEGIN_SEP
        dataDump (file, data_);
        END_SEP NEW_LINE
    }

    else
    {
        GO_ON_LEVEL(level)  BEGIN_SEP
        if (mode == PRINT_MODE::PRE_ORDER) dataDump (file, data_);
        NEW_LINE

        if (left_)
            left_ -> print (file, mode, compact, dataDump, beginSep, endSep, level + 1); 
        else
        {
            GO_ON_LEVEL(level + 1)  BEGIN_SEP END_SEP  NEW_LINE
        }

        if (mode == PRINT_MODE::IN_ORDER)
        {
            GO_ON_LEVEL(level)
            dataDump (file, data_);
            NEW_LINE
        }

        if (right_)
            right_ -> print (file, mode, compact, dataDump, beginSep, endSep, level + 1); 
        else
        {
            GO_ON_LEVEL(level + 1)  BEGIN_SEP END_SEP  NEW_LINE
        }

        GO_ON_LEVEL(level)
        if (mode == PRINT_MODE::POST_ORDER) dataDump (file, data_);

        END_SEP
        NEW_LINE else if (level == 0) fputc ('\n', file);
    }

#undef NEW_LINE
#undef BEGIN_SEP
#undef END_SEP
#undef GO_ON_LEVEL
}

void skipForChar_ (FILE* file, char c)
{
    int curr = fgetc (file);

    while (curr != EOF && curr != c)
        curr = fgetc (file);

    ungetc (curr, file);
    printf ("%c\n", curr);
}

void skipSpaces_ (FILE* file)
{
    int curr = fgetc (file);

    while (curr != EOF && (curr == ' ' || curr == '\n'))
        curr = fgetc (file);

    ungetc (curr, file);
}

Node* Node::read (FILE* file, PRINT_MODE mode, void* (*dataRead) (FILE* file), char beginSep, char endSep)
{
    assert (file);
    assert (dataRead);

    skipForChar_ (file, beginSep);
    fgetc (file);
    char c = fgetc (file);
    printf ("%c\n", c);

    if (c == endSep)
        return nullptr;

    else
    {
        ungetc (c, file);

        if (mode == PRINT_MODE::PRE_ORDER)
            data_ = dataRead (file);

        c = getc (file);
        if (c == endSep)
            return this;

        ungetc (c, file);

        skipForChar_ (file, beginSep);

        Node* leftNode = new Node (root_, nullptr);
        left_ = leftNode -> read (file, mode, dataRead, beginSep, endSep);
        if (left_ == nullptr) delete leftNode;

        if (mode == PRINT_MODE::IN_ORDER)
        {
            skipSpaces_ (file);
            data_ = dataRead (file);
        }

        skipForChar_ (file, beginSep);

        Node* rightNode = new Node (root_, nullptr);
        right_ = rightNode -> read (file, mode, dataRead, beginSep, endSep);
        if (right_ == nullptr) delete rightNode;

        if (mode == PRINT_MODE::POST_ORDER)
        {
            skipSpaces_ (file);
            data_ = dataRead (file);
        }

        skipForChar_ (file, endSep);
        fgetc (file);

        return this;
        
    }

    return nullptr;
}

void Tree::defaultDataDump (FILE* file, const void* data)
{
    if (data)
        fprintf (file, "%d", *((const int*) data));
    else
        fprintf (file, "");
}

void* Tree::defaultDataRead (FILE* file)
{
    void* data = new int (0);
    char c = fgetc (file);
    ungetc (c, file);
    printf ("%c\n", c);
    fscanf (file, "%d", data);
    return data;
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
                    return node -> setLeftNode (new Node (root_, data)) -> getData ();
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
                    return node -> setRightNode (new Node (root_, data)) -> getData ();
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
