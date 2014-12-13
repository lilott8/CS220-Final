#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#pragma once

#include "problem_object.h"
#include "algorithm.h"
#include "fortune.h"
#include "spm.h"

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
        void print_map();

    private:
        Map *kMap;
        Algorithm::AlgoType kAlgo;
        priority_queue<VNode*, vector<VNode*>, CloserToOrigin> kPins;
        //Fortune *kFortune;
        //SPM *kSPM;
        // This allows me to keep all my algorithms in one container!
        unique_ptr<Algorithm> kAlgorithm;
        Algorithm::Optimization kOpt;
    };
}
#endif