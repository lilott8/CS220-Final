#ifndef _SPC_H_
#define _SPC_H_

#pragma once

#include <vector>
#include <boost/polygon/voronoi_diagram.hpp>
#include "vnode.h"
#include "../Headers/btree.h"
#include "flow_algo.h"

using namespace Utilities;

namespace Flow {
    class SPC : public FlowAlgorithm {
    public:
        SPC();
        ~SPC();

        void start();
        void start(vector<VNode*>);
    private:
    };
}

#endif