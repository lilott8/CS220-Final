#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_

#pragma once

#include "arc.h"

namespace Flow {

    struct node {
        Arc *arc;
        node *left;
        node *right;
        bool is_traversable;
    };

    class BinaryTree {

    public:
        BinaryTree();
        ~BinaryTree();

        void insert(Arc*);
        Arc* search(Arc*);
        void destroy_tree();
        node* get_root();

    private:

        void destroy_tree(node*);
        void insert(Arc*, node*);
        node* search(Arc*, node*);
        node* kRoot;
    };
}
#endif