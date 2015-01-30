#include <math.h>
#include "../Headers/node.h"
#include "../Headers/VNode.h"

using namespace Flow;
using namespace Utilities;

VNode::VNode() {

}

VNode::VNode(double x, double y, int cost) {
    Node::set_x_coord(round(x));
    Node::set_y_coord(round(y));
    Node::set_cost(cost);
    this->kDoubleX = x;
    this->kDoubleY = y;
    this->kOutput = 0;
    this->kType = VNode::NONE;
}

VNode::VNode(Point p, int cost) {
    Node::set_coord(p);
    Node::set_cost(cost);
    this->kOutput = 0;
    this->kType = VNode::NONE;
}

VNode::~VNode() {
}

VNode::Type VNode::get_type() {
    return this->kType;
}

int VNode::get_output() {
    return this->kOutput;
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
    return this->kId;
}

double VNode::get_dx() {
    return this->kDoubleX;
}

double VNode::get_dy() {
    return this->kDoubleY;
}

string VNode::vnode_to_string() {
    string ret = "(" + to_string(get_x()) + ", " + to_string(get_y()) + ")";
    return ret;
}

