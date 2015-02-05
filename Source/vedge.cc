#include "../Headers/vedge.h"
#include "../Headers/vnode.h"
#include "../Headers/node.h"
#include "../Headers/claim.h"

using namespace Flow;

VEdge::VEdge(){}

VEdge::VEdge(VNode* start, VNode* left, VNode* right) {
    kStart = start;
    kEnd = NULL;

    kDirection = new VNode(right->get_y() - left->get_y(), -(right->get_x() - left->get_x()));
}

VEdge::~VEdge() {
    delete kDirection;
}

string VEdge::vedge_to_string() {
    string ret = "Starting node: " + kStart->vnode_to_string() + "\t->\t Ending Node: " + kEnd->vnode_to_string();
    return ret;
}