#ifndef _KRUSKAL_H_
#define _KRUSKAL_H_

#pragma once

#include <queue>
#include <vector>
#include "vnode.h"
#include "comparators.h"
#include "flow_algo.h"

using namespace Utilities;

namespace Flow {
    class Kruskal : public FlowAlgorithm {
    public:
        Kruskal();
        ~Kruskal();

        void start(priority_queue<VNode*, vector<VNode*>, CloserToOrigin>);

    private:

    };
}

#endif