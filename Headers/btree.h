#ifndef _BTREE_H_
#define _BTREE_H

#pragma once

#include "vnode.h"
#include "../Headers/node.h"

using namespace Utilities;
using namespace std;

namespace Flow {

    struct btree_node {
        VNode node;
        int distance;
        node *left;
        node *right;
    };

    class BinaryTree {
    public:
        BinaryTree();
        ~BinaryTree();

        void insert(VNode);
        btree_node *search(VNode);
        void destroy_tree();

    private:
        void destroy_tree(btree_node *);
        void insert(VNode, btree_node *);
        btree_node *searhc(VNode, btree_node *);

        btree_node *root;
    };
}

#endif