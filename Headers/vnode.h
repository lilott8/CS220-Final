#ifndef _VNode_H_
#define _VNode_H_

#pragma once

#include "node.h"

using namespace Utilities;
using namespace std;

namespace Flow {
    class VNode : public Node {

    public:
        enum Direction {NORTH, NORTH_EAST, EAST, SOUTH_EAST, SOUTH, SOUTH_WEST, WEST, NORTH_WEST};
        enum Type {PIN, BLOCKED, PATH, EDGE, NONE};

        VNode();
        VNode(int, int, int cost = 0);
        VNode(Point, int cost = 0);
        ~VNode();

        void set_neighbor(Direction, VNode*);
        void set_id(int);
        void set_type(Type);

        Type get_type();

        string static type_to_string(Type);
        string coords_to_string();

        int get_output();
    private:
        int id;
        int output;

        Type kType = Type::NONE;

    };
}
#endif