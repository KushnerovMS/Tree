#ifndef TREE_HEADER
#define TREE_HEADER

#include <stdlib.h>
#include <stdio.h>

#include "lib/GraphDump.h"

namespace Tree
{

#define ERRORS              \
    ERROR (NULL_ROOT)       \
    ERROR (NULL_ROOT_NODE)  \
    ERROR (NULL_SIZE)       \
    ERROR (ERROR_SIZE)      \
    ERROR (CICLDED)         \
    ERROR (DIFFERENT_ROOTS)

    class Error
    {
        public :

#define ERROR(name) unsigned int name : 1;
            ERRORS
#undef ERROR
#ifndef TREE_ERRORS_CPP
    #undef ERRORS
#endif

            explicit        Error   (unsigned long code);
            void            print   (FILE* file);
            unsigned long   getCode ();
            void            add     (Error err);
    };
    


    int defaultCmp (const void* a, const void* b);

    class Root
    {
        private:

            class Node* rootNode_;

            size_t size_;

            size_t dataSize_;
            int (*cmp_) (const void* a, const void* b);

        public:

            explicit Root (Node* rootNode,                                      /**< Pointer to node that will be root */
                           size_t dataSize,                                     /**< Size of data (if = 0, than data will not be copied) */
                           int (*comparator) (const void* a, const void* b)     /**< Function of data comparing */
                           );
            ~Root ();

            Node*   getRootNode ();

            size_t  getSize     ();
            size_t  incrSize    ();
            size_t  decrSize    ();

            size_t  getDataSize ();
            int     cmp         (const void* a, const void* b);
    };

    void defaultDataDump (FILE* file, const void* data);
    void* defaultDataRead (FILE* file, void* data);

    enum PRINT_MODE
    {
        PRE_ORDER   = 0,
        IN_ORDER    = 1,
        POST_ORDER  = 2

    };

    class Node
    {
        private:

            void* data_;

            Node* left_;
            Node* right_;

            Root* root_;

        public:

            explicit Node (void* data,
                           size_t dataSize,
                           int (*cmp) (const void* a, const void* b) = defaultCmp);
            explicit Node (Root* root, void* data);


            ~Node ();

            Error   notOK (size_t counter = 0);

            Node*   setLeftNode   (Node* node);
            Node*   setRightNode  (Node* node);


            Error   print       (FILE* file,
                                 PRINT_MODE mode,
                                 bool compact = false,
                                 void (*dataPrint) (FILE* file, const void* data) = defaultDataDump,
                                 const char* beginSeparator = "{",
                                 const char* endSeparator = "}",
                                 size_t level = 0);

            Error   dump        (PRINT_MODE mode,
                                 void (*dataPrint) (FILE* file, const void* data) = defaultDataDump);

            Error   graphDump   (const char* fileName = nullptr,
                                 void (*dataDump) (FILE* file, const void* data) = defaultDataDump);

            void    graphDump   (Graph* graph,
                                 void (*dataDump) (FILE* file, const void* data),
                                 Error err,
                                 size_t counter);
            
            Node*   read        (FILE* file,
                                 PRINT_MODE mode,
                                 void* (*dataRead) (FILE* file, void* data) = defaultDataRead,
                                 char beginSeparator = '{',
                                 char endSeparator = '}');

            Root*   getRoot     ();
            Node*   getLeftNode ();
            Node*   getRightNode();

            void*   getData     ();
            void    setData     (void* data);

            Node*   search      (void* data, bool create);
    };
}

#endif
