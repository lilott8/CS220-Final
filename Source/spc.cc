#include "../Headers/spc.h"
#include "../Headers/btree.h"
#include <vector>
#include <boost/polygon/voronoi_diagram.hpp>
#include <claim.h>

using namespace Flow;

SPC::SPC(){}

SPC::SPC(vector<VNode*> p, BinaryTree* btree) {
    kPins = p;
    kBTree = btree;
}

SPC::~SPC(){}


void SPC::start() {
    claim("S/start: starting SPC", kDebug);
    btree_node* found_node;

    kBTree->bfs();

    for(VNode* node : kPins) {
        found_node = kBTree->search_distance(node);
        if(found_node != NULL) {
            claim("S/start: " + node->vnode_to_string() + " is closest to: " + found_node->node->vnode_to_string(), kDebug);
        } else {
            claim("S/start: " + node->vnode_to_string() + " has no closest point...", kDebug);
        }
    }
}

void SPC::set_cells(vector<boost::polygon::voronoi_cell<double>> c) {
    kCells = c;
}