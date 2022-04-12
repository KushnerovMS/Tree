#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../lib/Logs.h"

#include "../Tree.h"

using namespace Tree;


Error Node::print (FILE* file, PRINT_MODE mode, bool compact, void (*dataDump) (FILE* file, const void* data), const char* beginSep, const char* endSep, size_t level)
{
    assert (file);
    assert (dataDump);
    assert (level >= 0);

    Error err (0);

    if (root_ == nullptr)
    {
        err.NULL_ROOT = 1;
        return err;
    }

    if (level > root_ -> getSize ())
    {
        err.CICLDED = 1;
        return err;
    }

#define NEW_LINE    if (! compact) fputc ('\n', file);
#define BEGIN_SEP   fprintf (file, "%s", beginSep);
#define END_SEP     fprintf (file, "%s", endSep);
#define GO_ON_LEVEL(lev) fprintf (file, "%*s", (int) ((lev) * 4 * (! compact)), "");

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
        {
            err = left_ -> print (file, mode, compact, dataDump, beginSep, endSep, level + 1); 
            if (err.getCode() != 0) return err;
        }
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
        {
            err = right_ -> print (file, mode, compact, dataDump, beginSep, endSep, level + 1); 
            if (err.getCode() != 0) return err;
        }
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

    return err;
}

Error Node::dump (PRINT_MODE mode,
                  void (*dataPrint) (FILE* file, const void* data))
{
    Logs.debug ("TreeNode [%p]", this);

    Error err = notOK();
    
    Logs.print ("<font color=%s><b>", (err.getCode() != 0) ? "red" : "green");
    err.print (Logs.getFile());
    Logs.print ("</b></font>\n");

    Logs.print ("{\n");

    Logs.print ("    <b>Root</b> [%p]\n", root_);

    if (root_)
    {
        Logs.print ("    {\n"
                    "        RootNode: %p\n"
                    "        Size: %lu\n"
                    "        DataSize: %lu\n"
                    "    }\n",
                    root_ -> getRootNode(),
                    root_ -> getSize(),
                    root_ -> getDataSize());
    }

    Logs.print ("    <b>Tree:</b>\n", root_);

    err.add (print (Logs.getFile(), mode, false, dataPrint, "{", "}", 1));

    Logs.print ("}\n");
    
    return err;
}


#define SKIP_SPACES                                     \
{                                                       \
    curr = fgetc (file);                                \
                                                        \
    while (curr != EOF && (curr == ' ' || curr == '\n'))\
        curr = fgetc (file);                            \
                                                        \
    ungetc (curr, file);                                \
}

#define SKIP_FOR_CHAR(c)                \
{                                       \
    curr = fgetc (file);                \
                                        \
    while (curr != EOF && curr != c)    \
        curr = fgetc (file);            \
                                        \
    ungetc (curr, file);                \
}

Node* Node::read (FILE* file, PRINT_MODE mode, void* (*dataRead) (FILE* file, void* data), char beginSep, char endSep)
{
    assert (file);
    assert (dataRead);

    int curr = 0;

    SKIP_FOR_CHAR (beginSep);
    fgetc (file);
    curr = fgetc (file);

    if (curr == endSep)
        return nullptr;
    else
    {
        ungetc (curr, file);

        if (mode == PRINT_MODE::PRE_ORDER)
            data_ = dataRead (file, data_);

        curr = getc (file);
        if (curr == endSep)
            return this;

        ungetc (curr, file);

        SKIP_FOR_CHAR (beginSep);

        Node* leftNode = new Node (root_, nullptr);
        left_ = leftNode -> read (file, mode, dataRead, beginSep, endSep);
        if (left_ == nullptr) delete leftNode;

        if (mode == PRINT_MODE::IN_ORDER)
        {
            SKIP_SPACES;
            data_ = dataRead (file, data_);
        }

        SKIP_FOR_CHAR (beginSep);

        Node* rightNode = new Node (root_, nullptr);
        right_ = rightNode -> read (file, mode, dataRead, beginSep, endSep);
        if (right_ == nullptr) delete rightNode;

        if (mode == PRINT_MODE::POST_ORDER)
        {
            SKIP_SPACES;
            data_ = dataRead (file, data_);
        }

        SKIP_FOR_CHAR (endSep);
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

void* Tree::defaultDataRead (FILE* file, void* data)
{
    fscanf (file, "%d", (int*) data);
    return data;
}
