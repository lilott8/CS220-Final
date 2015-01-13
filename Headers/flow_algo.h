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
#include "vevent.h"

using namespace Utilities;
using namespace std;

namespace Flow {
    class FlowAlgorithm {
    public:
        enum AlgoType {KRUSKAL, FORTUNE, SPM};      // algorithm enum
        enum Optimization {H_OPT, U_OPT, DEFAULT};  // kruskal's optimization enum
        FlowAlgorithm();                            // constructor
        virtual ~FlowAlgorithm();                   // virtual destructor

        string print_algo_type(AlgoType);           // debugging for which algorithm is being used
        string print_optimization(Optimization);    // debugging to let us know what we kruskals opt is used

        void set_map_size(int, int);                // Alert our class of the sizes of the map

        virtual void start(priority_queue<VNode*, vector<VNode*>, CloserToOrigin>); // run the algo
    private:

    protected:
        list<VNode*> kPlaces;       // list of vertices that exist
        list<VNode*> kPoints;       // Newly discovered points from the algorithm
        list<VEdge*> kEdges;        // generated edges

        VParabola* kRoot;           // Root of the beachline BTree

        int kWidth;                 // Width of our map
        int kHeight;                // height of our map

        void clear_all();           // clear the lists

    };

}

#endif