#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#pragma once

#include <string>
#include "vnode.h"
#include "vpath.h"
#include "comparators.h"
#include <queue>

using namespace Utilities;
using namespace std;

namespace Flow {
    class Algorithm {
    public:
        enum AlgoType {KRUSKAL, FORTUNE, SPM};
        enum Optimization {H_OPT, U_OPT, DEFAULT};
        Algorithm();
        virtual ~Algorithm();

        string print_algo_type(AlgoType);
        string print_optimization(Optimization);

        // Make this entire class abstract!
        virtual void start()=0;
    private:

    protected:
        vector<VNode*> kVertices;
        vector<VPath*> kEdges;
        priority_queue<VNode*, vector<VNode*>, CompareNodesFortune> kQueue();
    };

}

#endif