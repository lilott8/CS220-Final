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
    this->kPins = this->kMap->get_pins();
    kOpt = Algorithm::Optimization::DEFAULT;
    //kAlgorithm = new Fortune();
}

Controller::Controller(ProblemObject *po, Algorithm::AlgoType a, Algorithm::Optimization o) {
    // init our map
    this->kMap = new Map(po);
    // get the pins from the map.
    this->kPins = this->kMap->get_pins();
    // this will always be fortunes, this is the algorithm I'm implimenting
    this->set_algorithm(a);
    // set the optimization we will be using
    kOpt = o;
}

Controller::~Controller() {
}

void Controller::start() {
    claim("C/start: starting the algorithm", kNote);
    kAlgorithm->start();
}

void Controller::set_algorithm(Algorithm::AlgoType t) {
    kAlgo = t;
    switch(t) {
        default:
        case Algorithm::AlgoType::FORTUNE:
            claim("C/set_algorithm: using fortune", kNote);
            //kFortune = new Fortune();
            kAlgorithm = unique_ptr<Algorithm>(new Fortune());
            break;
        case Algorithm::AlgoType::SPM:
            claim("C/set_algorithm: using SPM", kNote);
            //kSPM = new SPM();
            kAlgorithm = unique_ptr<Algorithm>(new Fortune());
            break;
        case Algorithm::AlgoType::KRUSKAL:
            claim("C/set_algorithm: This shouldn't be kruskal!?", kWarning);
            break;
    }
}

void Controller::print_map() {
    this->kMap->print_map();
}