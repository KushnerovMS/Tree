#define TREE_ERRORS_CPP

#include "Tree.h"

Tree::Error::Error (unsigned long code)
{
#define ERROR(name) name = code & 1; code >>= 1;
ERRORS
#undef  ERROR
}

void Tree::Error::print (FILE* file)
{
    if (getCode() == 0)
        fprintf (file, "no_errors");
    else
    {
#define ERROR(name) if (name) fprintf (file, # name " ");
    ERRORS
#undef  ERROR
    }
}

unsigned long Tree::Error::getCode ()
{
    unsigned long code = 0;
#define ERROR(name) code |= name; code <<= 1;
    ERRORS
#undef  ERROR
    return code;
}

void Tree::Error::add (Error err)
{
#define ERROR(name) name |= err.name;
    ERRORS
#undef  ERROR
}

#undef ERRORS
#undef TREE_ERRORS_CPP
