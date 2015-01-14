#include "../Headers/vedge.h"
#include "../Headers/vnode.h"
#include "../Headers/node.h"
#include "../Headers/claim.h"

using namespace Flow;

VEdge::VEdge(){}

VEdge::VEdge(VNode* start, VNode* left, VNode* right) {
    kStart = start;
    kLeft = left;
    kRight = right;
    kNeighbor = NULL;
    kEnd = NULL;

    kF = (right->get_x() - left->get_x()) / (left->get_y() - right->get_y());
    kG = start->get_y() - kF * start->get_x();
    kDirection = new VNode(right->get_y() - left->get_y(), -(right->get_x() - left->get_x()));
}

VEdge::~VEdge() {
    delete kDirection;
}