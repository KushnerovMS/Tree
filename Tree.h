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

            int (*cmp_) (const void* a, const void* b);

        public:

            explicit Root (Node* rootNode, int (*cmp) (const void* a, const void* b) = nullptr);
            ~Root ();

            Node*   getRootNode ();

            size_t  getSize     ();
            size_t  incrSize    ();
            size_t  decrSize    ();

            int     cmp         (const void* a, const void* b);
    };

    int defaultDataDump (const void* data, FILE* file);

    class Node
    {
        private:

            void* data_;

            Node* left_;
            Node* right_;

            Root* root_;

        public:

            explicit Node (void* data, int (*cmp) (const void* a, const void* b) = nullptr);
            explicit Node (Root* root, void* data);
            ~Node ();

            Node*   addOnLeft   (void* data);
            Node*   addOnRight  (void* data);

            Error   dump        (FILE* file, int (*dataDump) (const void* data, FILE* file) = nullptr);

            Node*   getLeftNode ();
            Node*   getRightNode();

            void*   getData     ();
            void    setData     (void* data);

            void*   search      (void* data, bool create);
    };
}

#endif
