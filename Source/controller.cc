#include "../Headers/map.h"
#include "../Headers/controller.h"
#include "../Headers/spc.h"
#include "../Headers/node.h"
#include "../Headers/problem_object.h"
#include "../Headers/claim.h"
#include "../Headers/kruskal.h"
#include "../Headers/steiner.h"
#include <vector>


using namespace Utilities;
using namespace Algorithms;
using namespace Flow;

Controller::Controller() {

}

Controller::Controller(ProblemObject *po) {
    this->kMap = new Map(po);
    this->kPins = this->kMap->get_pins();
    kOpt = Controller::Optimization::DEFAULT;
    //kAlgorithm = new Fortune();
}

Controller::Controller(ProblemObject *po, Controller::AlgoType a, Controller::Optimization o) {
    // init our map
    this->kMap = new Map(po);
    // get the pins from the map.
    this->kPins = this->kMap->get_pins();
    // this will always be fortunes, this is the algorithm I'm implementing
    kSteiner = new Steiner(this->kMap);
    //this->set_algorithm(a);

    // set the optimization we will be using
    kOpt = o;
    //this->kSPC = SPC();
}

Controller::~Controller() {
    delete kSteiner;
}

void Controller::start() {
    kSteiner->start();
    //kMap->draw_voronoi_edges(kAlgorithm->get_edges());

    //this->project_vertices_on_map(kSteiner->get_edges());

    //this->kSPC = SPC();
    //this->kSPC.start(this->kMap->get_pins());

    kMap->print_map();
    //kKruskal.start();
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