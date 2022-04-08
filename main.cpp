#include <new>
#include <stdlib.h>
#include <time.h>

#include "lib/Logs.h"

#include "Tree.h"

int main ()
{
    int a = 10;
    Tree::Node* rootNode = new Tree::Node (&a, sizeof (int));

    //rootNode -> addOnLeft (new int (4)) -> addOnLeft (new int (2));
 
    srand (time (nullptr));

    for (int i = 0; i < 20; i ++)
    {
        a = rand () % 21;
        rootNode -> search (&a, true);
    }

    FILE* file = fopen ("Dump", "w");

    rootNode -> print (stdout, Tree::PRINT_MODE::PRE_ORDER);
    rootNode -> print (file, Tree::PRINT_MODE::PRE_ORDER);

    fclose (file);

    delete rootNode;

    rootNode = new Tree::Node (nullptr, 0, true);
    
    file = fopen ("Dump", "r");

    rootNode -> read (file, Tree::PRINT_MODE::PRE_ORDER);

    fclose (file);


    rootNode -> print (stdout, Tree::PRINT_MODE::PRE_ORDER);

    delete rootNode;


    putchar ('\n');

    return 0;
}
