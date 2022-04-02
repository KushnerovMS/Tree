#include <new>
#include <stdlib.h>
#include <time.h>

#include "lib/Logs.h"

#include "Tree.h"

int main ()
{
    int a = 10;
    Tree::Node* rootNode = new (std::nothrow) Tree::Node (&a, sizeof (int));

    char* buff = new char [10];
    delete [] buff;
    //rootNode -> addOnLeft (new int (4)) -> addOnLeft (new int (2));
 
    srand (time (nullptr));

    for (int i = 0; i < 20; i ++)
    {
        a = rand () % 21;
        rootNode -> search (&a, true);
    }

    //FILE* file = fopen ("Dump", "w");

    rootNode -> dump (stdout);

    //fclose (file);

    delete rootNode;

    putchar ('\n');

    return 0;
}
