#ifndef _FLOW_ALGO_H_
#define _FLOW_ALGO_H_

#pragma once

#include <string>
#include "vnode.h"
#include "vpath.h"
#include "comparators.h"
#include <queue>
#include "binary_tree.h"
#include <list>

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

        // Make this entire class abstract!
        virtual void start(priority_queue<VNode*, vector<VNode*>, CloserToOrigin>);
    private:

    protected:
        list<VNode*> kVertices;
        list<VPath*> kEdges;
        void clear_all();
    };

}

#endif