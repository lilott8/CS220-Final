#include "../Headers/btree.h"
#include "../Headers/vnode.h";
#include "../Headers/node.h"

using namespace Flow;
using namespace std;
using namespace Utilities;

BinaryTree::BinaryTree() {
    this->root = NULL;
}

void BinaryTree::destroy_tree(btree_node *leaf) {
    if(leaf!=NULL) {
        destroy_tree(leaf->left);
        destroy_tree(leaf->right);
        delete leaf;
    }
}

void BinaryTree::insert(VNode node, btree_node *leaf) {
    int distance = calculate_distance(node.get_coord());

    if(distance < leaf->distance) {
        if(leaf->left != NULL) {
            insert(node, leaf->left);
        } else {
            leaf->left = new node;
            leaf->left->node = node;
            leaf->left->left = NULL;
            leaf->left->right = NULL;
        }
    } else if(distance >= leaf->distance) {
        if(leaf->right != NULL) {
            insert(node, leaf->right);
        } else {
            leaf->right = new node;
            leaf->right->node = node;
            leaf->right->left = NULL;
            leaf->right->right = NULL;
        }
    }
}

