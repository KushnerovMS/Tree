#ifndef TREE_HEADER
#define TREE_HEADER

#include <stdlib.h>

namespace Tree
{

    enum Error
    {
        NO_ERRORS   = 0,
        BAD_DATA    = 1,
    };

    int defaultCmp (const void* a, const void* b);

    class Root
    {
        private:

            class Node* rootNode_;

            size_t size_;

            size_t dataSize_;
            bool destructDataWhileDestructing_;
            int (*cmp_) (const void* a, const void* b);

        public:

            explicit Root (Node* rootNode,                                      /**< Pointer to node that will be root */
                           size_t dataSize,                                     /**< Size of data (if = 0, than data will not be copied) */
                           bool destructDataWhileDestructing,                   /**< If true data will be deleted using delete() while tree destructing */
                           int (*cmp) (const void* a, const void* b) = nullptr  /**< Function of data comparing (if NULL, will be used Tree::defaultCmp */
                           );
            ~Root ();

            Node*   getRootNode ();

            size_t  getSize     ();
            size_t  incrSize    ();
            size_t  decrSize    ();
            bool    isDestructDataWhileDestructing ();

            size_t  getDataSize ();
            int     cmp         (const void* a, const void* b);
    };

    void defaultDataDump (FILE* file, const void* data);

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
                           bool destructDataWhileDestructing = false,
                           int (*cmp) (const void* a, const void* b) = nullptr);
            explicit Node (Root* root, void* data);
            ~Node ();

            Node*   addOnLeft   (Node* node);
            Node*   addOnRight  (Node* node);

            enum PRINT_MODE
            {
                PRE_ORDER   = 0,
                IN_ORDER    = 1,
                POST_ORDER  = 2

            };

            void    print       (FILE* file,
                                 PRINT_MODE mode,
                                 bool compact = false,
                                 void (*dataPrint) (FILE* file, const void* data) = defaultDataDump,
                                 const char* beginSeparator = "{",
                                 const char* endSeparator = "}",
                                 size_t level = 0);

            Root*   getRoot     ();
            Node*   getLeftNode ();
            Node*   getRightNode();

            void*   getData     ();
            void    setData     (void* data);

            void*   search      (void* data, bool create);
    };
}

#endif
