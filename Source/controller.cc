#include "../Headers/map.h"
#include "../Headers/controller.h"
#include "../Headers/spc.h"
#include "../Headers/node.h"
#include "../Headers/problem_object.h"
#include "../Headers/claim.h"
#include "../Headers/fortune.h"
#include "../Headers/flow_algo.h"
#include "../Headers/kruskal.h"
#include <vector>


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
    //this->kSPC = SPC();
}

Controller::~Controller() {
delete kAlgorithm;
}

void Controller::start() {
    kAlgorithm->set_euclidean(false);
    kAlgorithm->start(this->kMap->get_pins());
    //kMap->draw_voronoi_edges(kAlgorithm->get_edges());

    this->project_vertices_on_map(kAlgorithm->get_edges());

    kAlgorithm->run_queries();

    //this->kSPC = SPC();
    //this->kSPC.start(this->kMap->get_pins());

    kMap->print_map();
    //kKruskal.start();
}

void Controller::set_algorithm(FlowAlgorithm::AlgoType t) {
    kAlgo = t;
    switch(t) {
        default:
        case FlowAlgorithm::AlgoType::FORTUNE:
            //claim("C/set_algorithm: using fortune", kNote);
            //kFortune = new Fortune();
            //kAlgorithm = unique_ptr<FlowAlgorithm>(new Fortune());
            kAlgorithm = new Fortune();
            break;
        case FlowAlgorithm::AlgoType::SPM:
            claim("C/set_algorithm: SPM has been disabled, using Fortunes.", kWarning);
            //kSPM = new SPM();
            //kAlgorithm = unique_ptr<FlowAlgorithm>(new Fortune());
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

void Controller::project_vertices_on_map(vector<VEdge *> e) {
    vector<VEdge*> edges = e;
    for(int x = 0;x<(int)edges.size();x++) {
        kMap->set(edges.at(x)->kStart);
        kMap->set(edges.at(x)->kEnd);
    }
}