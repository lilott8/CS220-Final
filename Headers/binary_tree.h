#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_

#pragma once

#include "arc.h"

namespace Flow {

    class BinaryTree {
    public:
        BinaryTree();
        BinaryTree(VEvent*);
        ~BinaryTree();

        void insert(BinaryTree*);
        void search(BinaryTree*);
        void destroy_tree(BinaryTree*);

        BinaryTree* get_left_leaf();
        BinaryTree* get_right_leaf();
        void set_left_leaf(BinaryTree*);
        void set_right_leaf(BinaryTree*);
        void set_leaf_status(bool);
        void set_kedge(VEdge*);
        VEdge* get_kedge();
        bool is_leaf();
        VEvent* get_parabola_focus();

    private:
        // Flag for root or not
        bool kIsRoot;

        // Who is the leaf's parent?
        BinaryTree* kParent;

        // Who is the leaf's decendents?
        BinaryTree* kLeftLeaf;
        BinaryTree* kRightLeaf;

        // Focus point of our parabola
        VEvent* kParabolaFocus;

        // Where is the edge at?
        VEdge* kEdge;

        // Where did the parabola's intersect?
        VNode* kCircleEvent;
    };
}
#endif