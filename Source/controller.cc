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
    kOpt = Controller::Optimization::DEFAULT;
    //kAlgorithm = new Fortune();
}

Controller::Controller(ProblemObject *po, Controller::AlgoType a, Controller::Optimization o) {
    // init our map
    this->kMap = new Map(po);
    // get the pins from the map.
    kVertices = this->kMap->get_pins();

    // This begins our process
    kVoronoi = new Voronoi(this->kMap);

    //this->set_algorithm(a);

    // set the optimization we will be using
    kOpt = o;
    //this->kSPC = SPC();
}

Controller::~Controller() {
    delete kSteiner;
    delete kVoronoi;
}

void Controller::start() {
    /**
    * Paper 5 details a multi-step process to generate the OAVG
    *
    * 1) Generate the Voronoi Diagram (vertices become steiner points)
    * 2) add steiner points
    * 2) use prims algorithm to pick the best steiner points
    * 3) refine using algorithms in the paper
    */
    // Step 1
    kVoronoi->start();
    project_vertices_on_map(kVoronoi->get_edges());
    // add the nodes to our vertices
    vector<VNode*> temp = kVoronoi->get_vertices();
    claim("Number of vertices from voronoi: " + to_string(kVoronoi->get_vertices().size()), kDebug);
    for(int x = 0;x<temp.size();x++) { kVertices.push_back(temp.at(x));}

    // step 1.5 figure out further
    kSteiner = new Steiner();
    kSteiner->set_vertices(kVertices);
    kSteiner->start();

    //this->kSPC = SPC();
    //this->kSPC.start(this->kMap->get_pins());

    kMap->print_map();
    //kKruskal.start();
}


void Controller::print_map() {
    this->kMap->print_map();
}

void Controller::project_vertices_on_map(vector<VEdge *> e) {
    for(VEdge* v : e) {
        kMap->set(v->kStart);
        kMap->set(v->kEnd);
    }
}

int Controller::calculate_distance(int x, int y) {
    return (abs(x - 0) + abs(y - 0));
}