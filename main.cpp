#include <new>

#include "lib/Logs.h"

#ifndef LIB_MAKING
#include "Tree.h"
#endif

int main ()
{
    Tree::Node* rootNode = new (std::nothrow) Tree::Node (new int (5));

    rootNode -> addOnLeft (new int (4)) -> addOnLeft (new int (2));

    rootNode -> dump (stdout);

    delete rootNode;

    putchar ('\n');

    return 0;
}
