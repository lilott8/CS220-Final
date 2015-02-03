#ifndef _SPM_H_
#define _SPM_H_

#pragma once

#include "flow_algo.h"

using namespace Utilities;

namespace Flow {
    class SPM : public FlowAlgorithm {
    public:
        SPM();
        ~SPM();
        void start(vector<VNode*>);
    private:

    };
}

#endif