#ifndef _VNODE_H_
#define _VNODE_H_

#pragma once

#include "node.h"
#include "claim.h"

using namespace Utilities;
using namespace std;

/**
* Represents a point on a map
*/

namespace Flow {
    class VNode : public Node {

    public:
        // What direction is this map "pointing" (not used)
        enum Direction {NORTH, NORTH_EAST, EAST, SOUTH_EAST, SOUTH, SOUTH_WEST, WEST, NORTH_WEST};
        // What is the type of this node
        enum Type {PIN, BLOCKED, PATH, EDGE, NONE, STEINER, VORONOI, CORNER};

        VNode();
        VNode(double, double, int cost = 0);
        VNode(Point, int cost = 0);
        ~VNode();

        // Who is my neighbor
        void set_neighbor(Direction, VNode*);
        // set the unique id of this node
        void set_id(int);
        // set the type of this node
        void set_type(Type);
        // get the type of this node
        Type get_type();

        static int kLastId;                 // keeps track of the last ID

        // convert the node's type to a string value
        string static type_to_string(Type);
        // convert the point to a string
        string coords_to_string();
        // convert only the important data for .dot notation
        string vnode_to_dot();

        // not needed in this program
        int get_output();
        // return the id of the node
        int get_id();

        // double values if using euclidean space
        double get_dx();
        double get_dy();

        // conver the whole node to a string
        string vnode_to_string();

        // comparator needed for the set
        bool operator==(VNode&);

    private:
        int kId;                    // Unique id of this node, needed for the boost library
        int kOutput;                // not needed for this program
        double kDoubleX;            // Euclidean values
        double kDoubleY;            // Euclidean values

        Type kType = Type::NONE;    // What type is this node
    };
}
#endif