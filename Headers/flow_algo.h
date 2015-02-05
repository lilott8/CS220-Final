#ifndef _FLOW_ALGO_H_
#define _FLOW_ALGO_H_

#pragma once

#include <string>
#include <queue>
#include <list>
#include <queue>
#include "vnode.h"
#include "vpath.h"
#include "comparators.h"
#include "vedge.h"

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

        void set_map_size(int, int, int=0, int=0);  // Alert our class of the sizes of the map

        virtual void start(vector<VNode*>); // run the algo
        void set_euclidean(bool);

        vector<VEdge*> get_edges();
    private:

    protected:
        vector<VNode*> kPins;       // Pins that are placed and need to be routed
        vector<VEdge*> kEdges;      // generated edges

        int kMaxHeight;             // Max height of our map
        int kMaxWidth;              // Max width of our map
        int kMinWidth;              // Min width of our map
        int kMinHeight;             // Min height of map
        bool kIsEuclidean;          // Euclidean or Rectilinear space

        void clear_all();           // clear the lists
    };
}

#endif