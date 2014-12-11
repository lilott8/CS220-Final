#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#pragma once

#include "problem_object.h"
#include "algorithm.h"

using namespace Utilities;

namespace Flow {
    class Controller {
    public:
        Controller();
        Controller(ProblemObject*);
        Controller(ProblemObject*, Algorithm::AlgoType, Algorithm::Optimization);
        ~Controller();

        void start();
        void set_algorithm(Algorithm::AlgoType);
        void set_optimization(Algorithm::Optimization);

    private:
        Map *kMap;
        vector<VNode> kPins;
        Algorithm* kAlgorithm;
        Algorithm::Optimization kOpt;

        void set_pins(ProblemObject*);
    };
}
#endif