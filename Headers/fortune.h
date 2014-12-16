#ifndef _FORTUNE_H_
#define _FORTUNE_H_

#pragma once

#include "flow_algo.h"

using namespace Utilities;

namespace Flow {
    class Fortune : public FlowAlgorithm {
    public:
        Fortune();
        ~Fortune();
        void start(priority_queue<VNode*, vector<VNode*>, CloserToOrigin>);
    private:
    };
}

#endif