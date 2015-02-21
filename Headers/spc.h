#ifndef _SPC_H_
#define _SPC_H_

#pragma once

#include <vector>
#include <boost/polygon/voronoi_diagram.hpp>
#include "vnode.h"
#include "../Headers/btree.h"
#include "flow_algo.h"

using namespace Utilities;
using namespace Flow;

namespace Algorithms {
    class SPC {
    public:
        SPC();
        ~SPC();

        // Templated entry point for all algorithmic starts
        void start();
        void start(vector<VNode*>);
    private:
    };
}

#endif