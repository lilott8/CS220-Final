#ifndef _KRUSKAL_H_
#define _KRUSKAL_H_

#pragma once

#include <queue>
#include <vector>
#include "vnode.h"
#include "comparators.h"

using namespace Utilities;

namespace Flow {
    class Kruskal {
    public:
        Kruskal();
        ~Kruskal();

        void start(vector<VNode*>);

    private:

    };
}

#endif