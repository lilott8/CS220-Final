#ifndef _FLOW_ALGO_H_
#define _FLOW_ALGO_H_

#pragma once

#include <string>
#include <queue>
#include <list>
#include "vnode.h"
#include "vpath.h"
#include "comparators.h"
#include "vparabola.h"

using namespace Utilities;
using namespace std;

namespace Flow {
    class FlowAlgorithm {
    public:
        enum AlgoType {KRUSKAL, FORTUNE, SPM};
        enum Optimization {H_OPT, U_OPT, DEFAULT};
        FlowAlgorithm();
        virtual ~FlowAlgorithm();

        string print_algo_type(AlgoType);
        string print_optimization(Optimization);

        void set_map_size(int, int);

        virtual void start(priority_queue<VNode*, vector<VNode*>, CloserToOrigin>);
    private:

    protected:
        list<VNode*> kPlaces;       // list of vertices that exist
        list<VNode*> kPoints;       // Newly discovered points from the algorithm
        list<VEdge*> kEdges;        // generated edges

        VParabola* kRoot;           // Root of the beachline BTree

        int kWidth;                 // Width of our map
        int kHeight;                // height of our map

        void clear_all();

    };

}

#endif