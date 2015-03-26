#ifndef _HADLOCK_H_
#define _HADLOCK_H_

#pragma once

#include "vnode.h"
#include "map.h"
#include "voronoi.h"

using namespace Flow;
using namespace Utilities;

namespace FlowAlgorithms {
    class Hadlock {

        struct CompareNodes{
            bool operator()(VNode* &a, VNode* &b) {
                // do the checking here!
                return a->get_cost()  >= b->get_cost();
            }
        };

    public:
        Hadlock();
        Hadlock(Map*);
        ~Hadlock();

        void route(VNode*, VNode*);
    private:
        Map* kMap;
        VNode* kSource;
        VNode* kTarget;

        std::priority_queue<VNode*, vector<VNode*>, CompareNodes> kWaveFrontPQ;
        std::deque<VNode*> kTraceBack;
        std::deque<VNode*> kWaveFront;

        std::vector<VNode*> get_adjacent_nodes(VNode*);
        int calculate_metric(VNode*, VNode*);

        void route_recursive();

        bool is_placeable(VNode*);
        bool is_placeable(int, int);
        bool is_adjacent(VNode*, VNode*);

        bool is_in_queue(VNode*);
    };
}

#endif //_HADLOCK_H_
