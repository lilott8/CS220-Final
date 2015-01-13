#include "../Headers/vedge.h"
#include "../Headers/vnode.h"
#include "../Headers/node.h"

using namespace Flow;

VEdge::VEdge(){}

VEdge::VEdge(VNode* start, VNode* left, VNode* right) {
    kStart = start;
    kLeft = left;
    kRight = right;
    kNeighbor = NULL;
    kEnd = NULL;

    // Added this because there is, at least one case with my test data that the denominator is 0!
    // I'm not 100% certain what the denominator should be in the event of it equaling 0.
    if(left->get_y() - right->get_y() > 0) {
        kF = (right->get_x() - left->get_x()) / (left->get_y() - right->get_y());
    } else  {
        kF = 1;
    }
    kG = start->get_y() - kF * start->get_x();
    kDirection = new VNode(right->get_y() - left->get_y(), -(right->get_x() - left->get_x()));
}

VEdge::~VEdge() {
    delete kDirection;
}