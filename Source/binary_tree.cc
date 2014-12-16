#include <stddef.h>
#include "../Headers/binary_tree.h"

using namespace Flow;

BinaryTree::BinaryTree() {
    kRoot = NULL;
}

BinaryTree::~BinaryTree() {
    destroy_tree(kRoot);
}

void BinaryTree::destroy_tree(node *leaf) {
    if(leaf != NULL) {
        destroy_tree(leaf->left);
        destroy_tree(leaf->right);
        delete leaf;
    }
}

void BinaryTree::insert(Arc* c, node *leaf) {
    // It's less than so it goes left
    if(c->get_node()->get_x() == leaf->arc->get_node()->get_x()) {
        if(leaf->left != NULL) {
            insert(c, leaf->left);
        } else {
            leaf->left = new node;
            leaf->left->arc = c;
            leaf->left->left = NULL; // create NP's to new, non-existent nodes
            leaf->left->right = NULL; // create NP's to new, non-existent nodes
        }
        // if it's greater, go right!
    } else if(c->get_node()->get_x() > leaf->arc->get_node()->get_x()) {
        if (leaf->right != NULL) {
            insert(c, leaf->right);
        } else {
            leaf->right = new node;
            leaf->right->arc = c;
            leaf->right->left = NULL;
            leaf->right->right = NULL;
        }
        // If it's equal, then we will defer to the Y coordinate
    } else {
        // If it's less than the current y, then push it left
        if(c->get_node()->get_y() < leaf->arc->get_node()->get_y()) {
            if(leaf->left != NULL) {
                insert(c, leaf->left);
            } else {
                leaf->left = new node;
                leaf->left->arc = c;
                leaf->left->left = NULL; // create NP's to new, non-existent nodes
                leaf->left->right = NULL; // create NP's to new, non-existent nodes
            }
            // Otherwise, push it right
        } else {
            if (leaf->right != NULL) {
                insert(c, leaf->right);
            } else {
                leaf->right = new node;
                leaf->right->arc = c;
                leaf->right->left = NULL;
                leaf->right->right = NULL;
            }
        }
    }
}

node *BinaryTree::search(Arc *c, node *leaf) {
    if(leaf != NULL) {
        // found the node we need
        if (c->get_node()->get_x() == leaf->arc->get_node()->get_x()
                && c->get_node()->get_y() == leaf->arc->get_node()->get_y()) {
            return leaf;
        }
        if (c->get_node()->get_x() < leaf->arc->get_node()->get_x()) {
            return search(c, leaf->left);
        } else {
            return search(c, leaf->right);
        }
    } else {
        return NULL;
    }
}

void BinaryTree::insert(Arc* c) {
    if(kRoot != NULL) {
        insert(c, kRoot);
    } else {
        kRoot = new node;
        kRoot->arc = c;
        kRoot->left = NULL;
        kRoot->right = NULL;
    }
}

Arc* BinaryTree::search(Arc* c) {
    return search(c, kRoot)->arc;
}

void BinaryTree::destroy_tree() {
    destroy_tree(kRoot);
}

node* BinaryTree::get_root() {
    return kRoot;
}