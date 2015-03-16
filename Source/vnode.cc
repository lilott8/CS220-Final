#include <math.h>
#include <claim.h>
#include "../Headers/node.h"
#include "../Headers/VNode.h"

using namespace Flow;
using namespace Utilities;

int VNode::kLastId = 0;

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
    this->kId = kLastId;
    VNode::kLastId += 1;
}

VNode::VNode(Point p, int cost) {
    Node::set_coord(p);
    Node::set_cost(cost);
    this->kOutput = 0;
    this->kType = VNode::NONE;
    this->kId = kLastId;
    VNode::kLastId += 1;
}

VNode::~VNode() {
}

/**
* return the type of the node
*/
VNode::Type VNode::get_type() {
    return this->kType;
}

/**
* return the output field, not used here
*/
int VNode::get_output() {
    return this->kOutput;
}

/**
* set the type that the node is
*/
void VNode::set_type(Type t) {
    this->kType = t;
}

/**
* convert the type to string
*/
string VNode::type_to_string(Type t) {
    string output = "";
    switch(t) {
        case PIN:
            output = "pin";
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
        case STEINER:
            output = "s";
            break;
        case VORONOI:
            output = "v";
            break;
        case NONE:
        default:
            output = " ";
            break;
    }
    return output;
}

/**
* convert the points data structure to string
*/
string VNode::coords_to_string() {
    return "(" + std::to_string(this->get_x()) + ", "
            + std::to_string(this->get_y()) + ")";
}

/**
* return the id of the node
*/
int VNode::get_id() {
    return this->kId;
}

/**
* return the double-x value
*/
double VNode::get_dx() {
    return this->kDoubleX;
}

/**
* return the double-y value
*/
double VNode::get_dy() {
    return this->kDoubleY;
}

/**
* convert the node to a string
*/
string VNode::vnode_to_string() {
    string ret = "Meta: (ID: " + to_string(kId) + "\tType: " + type_to_string(kType) + ")\t Coords: (" + to_string(get_x()) + ", " + to_string(get_y()) + ")\t";
    return ret;
}

/**
* comparator for the set data structure
*/
bool VNode::operator==(VNode &other) {
    return ((this->get_x() == other.get_x()) && (this->get_y() == other.get_y()));
}