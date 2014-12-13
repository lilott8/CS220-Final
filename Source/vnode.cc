#include "../Headers/node.h"
#include "../Headers/VNode.h"

using namespace Flow;
using namespace Utilities;

VNode::VNode() {

}

VNode::VNode(int x, int y, int cost) {
    Node::set_x_coord(x);
    Node::set_y_coord(y);
    Node::set_cost(cost);
    this->output = 0;
    this->kType = VNode::NONE;
}

VNode::VNode(Point p, int cost) {
    Node::set_coord(p);
    Node::set_cost(cost);
    this->output = 0;
    this->kType = VNode::NONE;
}

VNode::~VNode() {
}

VNode::Type VNode::get_type() {
    return this->kType;
}

int VNode::get_output() {
    return this->output;
}

void VNode::set_type(Type t) {
    this->kType = t;
}

string VNode::type_to_string(Type t) {
    string output = "";
    switch(t) {
        case PIN:
            output = "p";
            break;
        case BLOCKED:
            output = "x";
            break;
        case PATH:
            output = "r";
            break;
        case EDGE:
            output = "e";
            break;
        case NONE:
        default:
            output = "0";
            break;
    }
    return output;
}

string VNode::coords_to_string() {
    return "(" + std::to_string(this->get_x()) + ", "
            + std::to_string(this->get_y()) + ")";
}

int VNode::get_id() {
    return this->id;
}

