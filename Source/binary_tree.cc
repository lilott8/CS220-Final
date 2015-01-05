#include <stddef.h>
#include "../Headers/binary_tree.h"

using namespace Flow;

BinaryTree::BinaryTree() {
    kParabolaFocus = NULL;
    kLeftLeaf = NULL;
    kRightLeaf = NULL;
    kIsRoot = true;
    kParent = NULL;
}

BinaryTree::BinaryTree(VEvent* b) {
    kParabolaFocus = b;
    kIsRoot = false;
    kRightLeaf = NULL;
    kLeftLeaf = NULL;
    kParent = NULL;
}

BinaryTree::~BinaryTree() {
}

void BinaryTree::destroy_tree(BinaryTree* root) {

}
bool BinaryTree::is_leaf() {
    return kIsRoot;
}


BinaryTree* BinaryTree::get_right_leaf() {
    return this->kRightLeaf;
}

BinaryTree* BinaryTree::get_left_leaf() {
    return this->kLeftLeaf;
}

void BinaryTree::set_left_leaf(BinaryTree* b) {
    kLeftLeaf = b;
    b->kParent = this;
}

void BinaryTree::set_right_leaf(BinaryTree* b) {
    kRightLeaf = b;
    b->kParent = this;
}

VEvent* BinaryTree::get_parabola_focus() {
    return kParabolaFocus;
}

void BinaryTree::set_leaf_status(bool b) {
    this->kIsRoot = b;
}

void BinaryTree::set_kedge(VEdge* v) {
    this->kEdge = v;
}

VEdge* BinaryTree::get_kedge() {
    return this->kEdge;
}