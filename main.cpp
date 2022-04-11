#include <new>
#include <stdlib.h>
#include <time.h>

#include "lib/Logs.h"

#include "Tree.h"

int main ()
{

    int a = 100;
    Tree::Node* rootNode = new Tree::Node (&a, sizeof (int));

    srand (time (nullptr));

    for (int i = 0; i < 200; i ++)
    {
        a = rand () % 200;
        rootNode -> search (&a, true);
    }

    FILE* file = fopen ("Dump", "w");

    rootNode -> print (file, Tree::PRINT_MODE::PRE_ORDER);

    fclose (file);

    delete rootNode;

    rootNode = new Tree::Node (nullptr, sizeof (void*));
    
    file = fopen ("Dump", "r");

    rootNode -> read (file, Tree::PRINT_MODE::PRE_ORDER);

    fclose (file);


    rootNode -> print (stdout, Tree::PRINT_MODE::PRE_ORDER);

    rootNode -> dump (Tree::PRINT_MODE::PRE_ORDER);

    rootNode -> graphDump ("testDump.svg");

    delete rootNode;

    putchar ('\n');

    return 0;
}
