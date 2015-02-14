#include <claim.h>
#include "../Headers/btree.h"
#include "../Headers/vnode.h";
#include "../Headers/node.h"

using namespace Flow;
using namespace std;
using namespace Utilities;

BinaryTree::BinaryTree() {
    this->kRoot = NULL;
}

BinaryTree::~BinaryTree() {
    destroy_tree();
}

void BinaryTree::insert(VNode* node){
    if(kRoot!=NULL) {
        insert(node, kRoot);
    } else {
        kRoot = new btree_node();
        kRoot->node = node;
        kRoot->left = NULL;
        kRoot->right = NULL;
        kRoot->distance = calculate_distance(node->get_coord());
        kSize += 1;
    }
}

btree_node* BinaryTree::search_distance(VNode* node) {
    bool whichOne = false;
    if(kRoot != NULL) {
        if(whichOne) {
            return search_recursive(node, kRoot);
        } else {
            return search_iterative(node, kRoot);
        }
    }
    return NULL;
}

void BinaryTree::destroy_tree() {
    destroy_tree(kRoot);
}

int BinaryTree::get_size(){
    return kSize;
}

void BinaryTree::bfs() {
    string output="B/bfs: ";
    kQueue.push_back(kRoot);

    btree_node* node;
    int x = 0;
    while(!kQueue.empty()) {
        node = kQueue.front();
        kQueue.pop_front();

        if(node != NULL) {
            if (node->left != NULL) {
                kQueue.push_back(node->left);
            }
            if (node->right != NULL) {
                kQueue.push_back(node->right);
            }
            output += node->node->vnode_to_string();
        }

        if(x < kSize-1) {
            output += "\t->\t";
        }
        x++;
    }
    claim(output, kDebug);
}

vector<VNode*> BinaryTree::get_bfs() {
    vector<VNode*> queue;
    kQueue.push_back(kRoot);
    queue.push_back(kRoot->node);

    btree_node* node;
    int x = 0;
    while(!kQueue.empty()) {
        node = kQueue.front();
        kQueue.pop_front();
        if (node != NULL) {
            if (node->left != NULL) {
                kQueue.push_back(node->left);
                queue.push_back(node->left->node);
            }
            if (node->right != NULL) {
                kQueue.push_back(node->right);
                queue.push_back(node->right->node);
            }
        }
    }
    return queue;
}

void BinaryTree::dfs() {
    string output="B/dfs: ";
    kQueue.push_back(kRoot);

    btree_node* node;
    int x = 0;
    while(!kQueue.empty()) {
        node = kQueue.back();
        kQueue.pop_back();

        if(node->left != NULL) {
            kQueue.push_back(node->left);
        }
        if(node->right != NULL) {
            kQueue.push_back(node->right);
        }

        output += node->node->vnode_to_string();
        if(x < kQueue.size()-1) {
            output += "\t->\t";
        }
    }
    claim(output, kDebug);
}


void BinaryTree::destroy_tree(btree_node *leaf) {
    if(leaf!=NULL) {
        destroy_tree(leaf->left);
        destroy_tree(leaf->right);
        delete leaf;
    }
}

void BinaryTree::insert(VNode* node, btree_node *leaf) {
    int distance = calculate_distance(node->get_coord());

    // We don't want to insert the same edge multiple times.
    if(node->get_x() == leaf->node->get_x() && node->get_y() == leaf->node->get_y()) {
        //claim("BT/insert: we are attempting to insert a duplicate", kDebug);
        return;
    }

    if(node->get_x() < leaf->node->get_x()) {
        if(leaf->left != NULL) {
            insert(node, leaf->left);
        } else {
            leaf->left = new btree_node();
            leaf->left->node = node;
            leaf->distance = distance;
            leaf->left->left = NULL;
            leaf->left->right = NULL;
            kSize += 1;
        }
    } else if(node->get_x() > leaf->node->get_x()) {
        if(leaf->right != NULL) {
            insert(node, leaf->right);
        } else {
            leaf->right = new btree_node();
            leaf->right->node = node;
            leaf->distance = distance;
            leaf->right->left = NULL;
            leaf->right->right = NULL;
            kSize += 1;
        }
    } else {
        if(node->get_y() < leaf->node->get_y()) {
            if(leaf->left != NULL) {
                insert(node, leaf->left);
            } else {
                //add the new node!
                leaf->left = new btree_node();
                leaf->left->node = node;
                leaf->distance = distance;
                leaf->left->left = NULL;
                leaf->left->right = NULL;
                kSize += 1;
            }
        } else {
            if(leaf->right != NULL) {
                insert(node, leaf->right);
            } else {
                // add the new node!
                leaf->right = new btree_node();
                leaf->right->node = node;
                leaf->distance = distance;
                leaf->right->left = NULL;
                leaf->right->right = NULL;
                kSize += 1;
            }
        }
    }
}

btree_node* BinaryTree::search_recursive(VNode* node, btree_node* leaf) {
    int distance = calculate_distance(node->get_coord());

    if(leaf->left != NULL && (node->get_x() < leaf->node->get_x())) {
        return search_recursive(node, leaf->left);
    } else if(leaf->right != NULL && (node->get_x() > leaf->node->get_x())) {
        return search_recursive(node, leaf->right);
    } else {

            if(node->get_y() < leaf->node->get_y()) {
                return search_recursive(node, leaf->left);
            } else {
                return search_recursive(node, leaf->right);
            }
        }
}

/**
* This is kind of working.  I have to first project the vertices onto the map to verify
*/
btree_node* BinaryTree::search_iterative(VNode* node, btree_node* leaf) {
    if(kRoot == NULL) {
        return NULL;
    }

    claim("BT/search_iterative: ==================================================", kDebug);
    claim("BT/search_iterative: Node: " + node->vnode_to_string(), kDebug);
    claim("BT/searhc: Root node: " + kRoot->node->vnode_to_string(), kDebug);

    btree_node* curr = kRoot;
    int node_distance = calculate_distance(node->get_coord());
    int best_guess = abs(kRoot->distance - node_distance);
    btree_node* best_guess_node = kRoot;

    claim("BT/search_iterative: Current node " + node->vnode_to_string() + "\tdistance is: " + to_string(node_distance), kDebug);

    int x = 0;

    do{
        if(node->get_x() < curr->node->get_x()) {
            claim("BT/search: Changing node from: " + curr->node->vnode_to_string() + " to " + curr->left->node->vnode_to_string(), kDebug);
            curr = curr->left;
        } else {
            claim("BT/search: Changing node from: " + curr->node->vnode_to_string() + " to " + curr->right->node->vnode_to_string(), kDebug);
            curr = curr->right;
        }

        // Change the best guess only if the next best guess is lower!
        if(abs(curr->distance - node_distance) < best_guess) {
            best_guess = abs(curr->distance - node_distance);
            best_guess_node = curr;
            //claim("Setting the best guess stuff!", kDebug);
        }
        x++;
    } while(curr->left != NULL && curr->right != NULL);

    claim("BT/search_iterative: Done searching after: "+to_string(x)+ " iterations", kDebug);
    claim("BT/search_iterative: ==================================================", kDebug);
    return best_guess_node;
}

int BinaryTree::calculate_distance(Point p) {
    return (abs(p.x - 0) + abs(p.y - 0));
}