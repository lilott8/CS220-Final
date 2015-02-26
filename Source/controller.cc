#include "../Headers/map.h"
#include "../Headers/controller.h"
#include "../Headers/spc.h"
#include "../Headers/node.h"
#include "../Headers/problem_object.h"
#include "../Headers/claim.h"
#include "../Headers/steiner.h"
#include "../Headers/prim.h"
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
}

Controller::~Controller() {
    delete kSteiner;
    delete kVoronoi;
    delete kPrim;
    delete kSPC;
}

void Controller::start() {
    /**
    * Paper 5 details a multi-step process to generate the OAVG
    *
    * 1) Generate the Voronoi Diagram (vertices become steiner points)
    * 2) add steiner points
    * 3) use Dijkstra's/Kruskal's algorithm to pick the best steiner points
    * 4) refine using algorithms in the paper
    */
    // Step 1
    kVoronoi->start();
    // add the nodes to our vertices
    vector<VNode*> temp = kVoronoi->get_vertices();
    claim("C/start: Number of vertices from voronoi: " + to_string(kVoronoi->get_vertices().size()), kDebug);
    // Combine the voronoie with the pins
    for(int x = 0;x<(int)temp.size();x++) { kVertices.push_back(temp.at(x));}
    project_vertices_on_map(kVertices);

    // step 2
    kSteiner = new Steiner();
    kSteiner->set_vertices(kVertices);
    kSteiner->start();
    vector<VNode*> sp = kSteiner->get_steiner_points();

    project_vertices_on_map(sp);
    // Step 3
    this->kSPC = new SPC();
    this->kSPC->start();

    kMap->print_map();
}


void Controller::print_map() {
    this->kMap->print_map();
}

void Controller::project_edges_on_map(vector<VEdge *> e) {
    for(VEdge* v : e) {
        kMap->set(v->kStart);
        kMap->set(v->kEnd);
    }
}

void Controller::project_vertices_on_map(vector<VNode *> n) {
    for(VNode* v : n) {
        kMap->set(v);
    }
}

int Controller::calculate_distance(int x, int y) {
    return (abs(x - 0) + abs(y - 0));
}