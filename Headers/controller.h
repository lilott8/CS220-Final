#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#pragma once

#include "problem_object.h"
#include "flow_algo.h"
#include "fortune.h"
#include "spm.h"

using namespace Utilities;

namespace Flow {
    class Controller {
    public:
        Controller();
        Controller(ProblemObject*);
        Controller(ProblemObject*, FlowAlgorithm::AlgoType, FlowAlgorithm::Optimization);
        ~Controller();

        void start();
        void set_algorithm(FlowAlgorithm::AlgoType);
        void set_optimization(FlowAlgorithm::Optimization);
        void print_map();

    private:
        Map *kMap;
        FlowAlgorithm::AlgoType kAlgo;
        priority_queue<VNode*, vector<VNode*>, CloserToOrigin> kPins;
        //Fortune *kFortune;
        //SPM *kSPM;
        // This allows me to keep all my algorithms in one container!
        unique_ptr<FlowAlgorithm> kAlgorithm;
        FlowAlgorithm::Optimization kOpt;

    };
}
#endif