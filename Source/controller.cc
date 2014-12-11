#include "../Headers/spm.h"
#include "../Headers/map.h"
#include "../Headers/controller.h"
#include "../Headers/node.h"
#include "../Headers/problem_object.h"
#include "../Headers/claim.h"
#include "../Headers/fortune.h"
#include "../Headers/algorithm.h"

using namespace Utilities;
using namespace Flow;

Controller::Controller() {

}

Controller::Controller(ProblemObject *po) {
    this->kMap = new Map(po);
    //set_pins(po);
    kOpt = Algorithm::Optimization::DEFAULT;
    kAlgorithm = new Fortune();
}

Controller::Controller(ProblemObject *po, Algorithm::AlgoType a, Algorithm::Optimization o) {
    this->kMap = new Map(po);
    this->set_algorithm(a);
    kOpt = o;
}

Controller::~Controller() {
    delete kAlgorithm;
}

void Controller::start() {
    claim("Starting to run the controller!", kNote);
}

void Controller::set_algorithm(Algorithm::AlgoType t) {
    switch(t) {
        default:
        case Algorithm::AlgoType::FORTUNE:
            claim("using fortune", kNote);
            kAlgorithm = new Fortune();
            break;
        case Algorithm::AlgoType::SPM:
            claim("using SPM", kNote);
            kAlgorithm = new SPM();
            break;
        case Algorithm::AlgoType::KRUSKAL:
            claim("This shouldn't be kruskal!?", kWarning);
            break;
    }
}

void Controller::set_pins(ProblemObject *po) {
    Route route;
    int c = po->get_connections().size();

    for(int x = 0;x < c; x++) {
        /**
        * TODO: add error handling for placing of pins
        */
        // Declare the pin(s)
        VNode temp = VNode(po->get_connections().at(x).source);
        temp.set_type(VNode::Type::PIN);
        claim("Source: " + temp.coords_to_string(), kDebug);
        //kMap.at(temp.get_x()).at(temp.get_y())->set_type(LeeNode::NodeType::SOURCE);
        route.pStart = temp;
        kPins.push_back(temp);

        // Declare the pin(s)
        temp = VNode(po->get_connections().at(x).sink);
        temp.set_type(VNode::Type::PIN);
        claim("sink: " + temp.coords_to_string(), kDebug);
        //kMap.at(temp.get_x()).at(temp.get_y())->set_type(LeeNode::NodeType::SOURCE);
        route.pStop = temp;
        kPins.push_back(temp);

        //kMap.at(route.pStart.get_x()).at(route.pStart.get_y())->set_type(VNode::Type::PIN);
        //kMap.at(route.pStop.get_x()).at(route.pStop.get_y())->set_type(VNode::Type::PIN);
    }
}