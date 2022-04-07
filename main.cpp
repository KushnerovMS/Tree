#include <new>
#include <stdlib.h>
#include <time.h>

#include "lib/Logs.h"

#include "Tree.h"

int main ()
{
    int a = 10;
    Tree::Node* rootNode = new (std::nothrow) Tree::Node (&a, sizeof (int));

    //rootNode -> addOnLeft (new int (4)) -> addOnLeft (new int (2));
 
    srand (time (nullptr));

    for (int i = 0; i < 20; i ++)
    {
        a = rand () % 21;
        rootNode -> search (&a, true);
    }

    //FILE* file = fopen ("Dump", "w");

    rootNode -> print (stdout, Tree::Node::PRINT_MODE::IN_ORDER, true);
    rootNode -> print (stdout, Tree::Node::PRINT_MODE::IN_ORDER);

    printf ("%p %p\n", rootNode -> getLeftNode (), rootNode -> getRightNode ());

    //fclose (file);

    delete rootNode;

    putchar ('\n');

    return 0;
}
