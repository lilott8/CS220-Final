#include "../Headers/spm.h"
#include "../Headers/map.h"
#include "../Headers/controller.h"
#include "../Headers/node.h"
#include "../Headers/problem_object.h"
#include "../Headers/claim.h"
#include "../Headers/fortune.h"
#include "../Headers/flow_algo.h"
#include "../Headers/kruskal.h"


using namespace Utilities;
using namespace Flow;

Controller::Controller() {

}

Controller::Controller(ProblemObject *po) {
    this->kMap = new Map(po);
    this->kPins = this->kMap->get_pins();
    kOpt = FlowAlgorithm::Optimization::DEFAULT;
    //kAlgorithm = new Fortune();
}

Controller::Controller(ProblemObject *po, FlowAlgorithm::AlgoType a, FlowAlgorithm::Optimization o) {
    // init our map
    this->kMap = new Map(po);
    // get the pins from the map.
    this->kPins = this->kMap->get_pins();
    // this will always be fortunes, this is the algorithm I'm implementing
    this->set_algorithm(a);
    // set the optimization we will be using
    kOpt = o;
    // Make sure the size of the map is known to the algorithm classes
    kAlgorithm->set_map_size((double)this->kMap->get_x(), (double)this->kMap->get_y());
}

Controller::~Controller() {
}

void Controller::start() {
    kAlgorithm->set_euclidean(false);
    kAlgorithm->start(this->kMap->get_pins());
    project_edges_on_map(kAlgorithm->get_edges());
    //kKruskal.start();
}

void Controller::set_algorithm(FlowAlgorithm::AlgoType t) {
    kAlgo = t;
    switch(t) {
        default:
        case FlowAlgorithm::AlgoType::FORTUNE:
            //claim("C/set_algorithm: using fortune", kNote);
            //kFortune = new Fortune();
            kAlgorithm = unique_ptr<FlowAlgorithm>(new Fortune());
            break;
        case FlowAlgorithm::AlgoType::SPM:
            //claim("C/set_algorithm: using SPM", kNote);
            //kSPM = new SPM();
            kAlgorithm = unique_ptr<FlowAlgorithm>(new Fortune());
            break;
        case FlowAlgorithm::AlgoType::KRUSKAL:
            claim("C/set_algorithm: This shouldn't be kruskal!?", kWarning);
            kKruskal = Kruskal();
            break;
    }
}

void Controller::print_map() {
    this->kMap->print_map();
}

void Controller::project_edges_on_map(vector<VEdge*> edges) {
    bool which_x, which_y;
    int start_x, start_y, end_x, end_y;

    VEdge* edge;
    for(int x = 0;x<edges.size();x++) {
        edge = edges.at(x);
        which_x = (edge->kStart->get_x() < edge->kEnd->get_x());
        which_y = (edge->kStart->get_y() < edge->kEnd->get_y());

        start_x = which_x ? edge->kStart->get_x() : edge->kEnd->get_x();
        start_y = which_x ? edge->kStart->get_y() : edge->kEnd->get_y();

        end_x = !which_x ? edge->kStart->get_x() : edge->kEnd->get_x();
        end_y = !which_x ? edge->kStart->get_y() : edge->kEnd->get_y();

    }
}