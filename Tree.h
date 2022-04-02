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

            size_t dataSise_;
            int (*cmp_) (const void* a, const void* b);

        public:

            explicit Root (Node* rootNode, size_t dataSize, int (*cmp) (const void* a, const void* b) = nullptr);
            ~Root ();

            Node*   getRootNode ();

            size_t  getSize     ();
            size_t  incrSize    ();
            size_t  decrSize    ();

            size_t  getDataSize ();
            int     cmp         (const void* a, const void* b);
    };

    void defaultDataDump (const void* data, FILE* file);

    class Node
    {
        private:

            void* data_;

            Node* left_;
            Node* right_;

            Root* root_;

        public:

            explicit Node (void* data, size_t dataSize, int (*cmp) (const void* a, const void* b) = nullptr);
            explicit Node (Root* root, void* data);
            ~Node ();

            Node*   addOnLeft   (void* data);
            Node*   addOnRight  (void* data);

            void    dump        (FILE* file, void (*dataDump) (const void* data, FILE* file) = nullptr, size_t level = 0);

            Node*   getLeftNode ();
            Node*   getRightNode();

            void*   getData     ();
            void    setData     (void* data);

            void*   search      (void* data, bool create);
    };
}

#endif
