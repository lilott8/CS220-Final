#include <controller.h>
#include "../Headers/vedge.h"
#include "../Headers/vnode.h"
#include "../Headers/node.h"
#include "../Headers/claim.h"

using namespace Flow;

int VEdge::kLastId = 0;

VEdge::VEdge(){}

VEdge::VEdge(VNode* start, VNode* left, VNode* right) {
    kStart = start;
    kEnd = NULL;
    kCost = -1;
    kId = kLastId;
    kLastId += 1;

    kDirection = new VNode(right->get_y() - left->get_y(), -(right->get_x() - left->get_x()));
}

VEdge::VEdge(VNode* start, VNode* end) {
    kStart = start;
    kEnd = end;
    kId = kLastId;
    kLastId += 1;
    kCost = Controller::calculate_manhattan_distance(start, end);
}

VEdge::~VEdge() {
    delete kDirection;
}

string VEdge::vedge_to_string() {
    string ret = "Id: " + to_string(kId) + " Starting node: " + kStart->vnode_to_string()
            + "\t->\t Ending Node: " + kEnd->vnode_to_string() + " with a cost of: " + to_string(kCost);
    return ret;
}

bool VEdge::operator==(VEdge &other) {
    return ((this->kStart == other.kStart) && (this->kEnd == other.kEnd));
}