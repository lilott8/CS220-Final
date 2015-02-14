#ifndef _BTREE_H_
#define _BTREE_H_

#pragma once

#include "vnode.h"
#include "../Headers/node.h"
#include <deque>

using namespace Utilities;
using namespace std;

namespace Flow {

    struct btree_node {
        VNode* node;
        int distance;
        btree_node *left;
        btree_node *right;
    };

    class BinaryTree {
    public:
        BinaryTree();
        ~BinaryTree();

        void insert(VNode*);
        void destroy_tree();
        void dfs();
        void bfs();

        vector<VNode*> get_bfs();

        btree_node *search_distance(VNode*);

        int get_size();

    private:
        void destroy_tree(btree_node *);
        void insert(VNode*, btree_node *);

        btree_node* search_recursive(VNode*, btree_node*);
        btree_node* search_iterative(VNode*, btree_node*);

        int calculate_distance(Point);

        deque<btree_node*> kQueue;
        btree_node *kRoot;
        int kSize = 0;
    };
}

#endif