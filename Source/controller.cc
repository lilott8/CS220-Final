#include "../Headers/map.h"
#include "../Headers/controller.h"
#include "../Headers/spc.h"
#include "../Headers/node.h"
#include "../Headers/problem_object.h"
#include "../Headers/claim.h"
#include "../Headers/steiner.h"
#include "../Headers/prim.h"
#include <vector>
#include <set>
#include "../Headers/graph_alogs.h"

using namespace Utilities;
using namespace FlowAlgorithms;
using namespace Flow;
using namespace std;

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
    //delete kSPC;
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
    std::set<VNode*> temp = kVoronoi->get_vertices();
    claim("C/start: Number of vertices from voronoi: " + to_string(kVoronoi->get_vertices().size()), kDebug);
    // Combine the voronoie with the pins
    for(VNode* v : temp) {kVertices.insert(v);}
    project_vertices_on_map(kVertices);

    // step 2
    kSteiner = new Steiner(kVertices);
    kSteiner->start();
    set<VNode*> sp = kSteiner->get_steiner_points();
    claim("there are " + to_string(sp.size()) + " steiner points", kDebug);
    claim("there are " + to_string(kSteiner->get_steiner_edges().size()) + " steiner edges", kDebug);

    //project_vertices_on_map(sp);
    // Step 3
    GraphAlgos* ga = new GraphAlgos(kSteiner->get_steiner_edges());
    ga->start();

    //this->kSPC = new SPC(kSteiner->get_steiner_edges());
    //this->kSPC->start();

    //kMap->print_map();
}


void Controller::print_map() {
    this->kMap->print_map();
}

void Controller::project_edges_on_map(set<VEdge *> e) {
    for(VEdge* v : e) {
        kMap->set(v->kStart);
        kMap->set(v->kEnd);
    }
}

void Controller::project_vertices_on_map(set<VNode *> n) {
    for(VNode* v : n) {
        kMap->set(v);
    }
}

int Controller::calculate_distance(int x, int y) {
    return (abs(x - 0) + abs(y - 0));
}

int Controller::calculate_manhattan_distance(VNode* a, VNode* b) {
    int order1, order2;
    order1 = abs(a->get_x() - b->get_x());
    order2 = abs(a->get_y() - b->get_y());
    return order1 + order2;
}