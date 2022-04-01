#include <new>

#include "lib/Logs.h"

#ifndef LIB_MAKING
#include "Tree.h"
#endif

int main ()
{
    Tree::Node* rootNode = new (std::nothrow) Tree::Node (new int (5));

    //rootNode -> addOnLeft (new int (4)) -> addOnLeft (new int (2));

    rootNode -> search (new int (4), 1);
    rootNode -> search (new int (8), 1);
    rootNode -> search (new int (9), 1);
    rootNode -> search (new int (6), 1);


    FILE* file = fopen ("Dump", "w");

    rootNode -> dump (file);

    fclose (file);

    delete rootNode;

    putchar ('\n');

    return 0;
}
