#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#pragma once

#include <string>
#include "node.h"

using namespace Utilities;
using namespace std;

namespace Flow {
    class Algorithm {
    public:
        enum AlgoType {KRUSKAL, FORTUNE, SPM};
        enum Optimization {H_OPT, U_OPT, DEFAULT};
        Algorithm();
        ~Algorithm();

        string print_algo_type(AlgoType);
        string print_optimization(Optimization);
        // stuff
    private:
        // stuff
    };

}

#endif