#ifndef _VNODE_H_
#define _VNODE_H_

#pragma once

#include "node.h"

using namespace Utilities;
using namespace std;

namespace Flow {
    class VNode : public Node {
    public:
        enum Direction {NORTH, NORTH_EAST, EAST, SOUTH_EAST, SOUTH, SOUTH_WEST, WEST, NORTH_WEST};
        enum Type {PIN, BLOCKED, PATH, EDGE, NONE, STEINER};

        VNode();
        VNode(double, double, int cost = 0);
        VNode(Point, int cost = 0);
        ~VNode();

        void set_neighbor(Direction, VNode*);
        void set_id(int);
        void set_type(Type);

        Type get_type();

        string static type_to_string(Type);
        string coords_to_string();

        int get_output();
        int get_id();

        double get_dx();
        double get_dy();

    private:
        int kId;
        int kOutput;
        double kDoubleX;
        double kDoubleY;

        Type kType = Type::NONE;
    };
}
#endif