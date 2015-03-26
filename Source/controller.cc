#include "../Headers/map.h"
#include "../Headers/controller.h"
#include "../Headers/hadlock.h"
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

/**
* Alternate constructor
*/
Controller::Controller(ProblemObject *po) {
    this->kMap = new Map(po);
    kOpt = Controller::Optimization::DEFAULT;
    //kAlgorithm = new Fortune();
    kHadlock = new Hadlock(kMap);
}

/**
* Alternate constructor
*/
Controller::Controller(ProblemObject *po, Controller::AlgoType a, Controller::Optimization o) {
    // init our map
    this->kMap = new Map(po);
    // get the pins from the map.
    kVertices = this->kMap->get_pins();

    // Make sure Voronoi has a pointer to the map
    kVoronoi = new Voronoi(this->kMap);
    // Make sure Hadlock has a pointer to the map
    kHadlock = new Hadlock(kMap);

    //this->set_algorithm(a);

    // set the optimization we will be using
    kOpt = o;
}

/**
* Destructor
*/
Controller::~Controller() {
    //delete kSteiner;
    //delete kVoronoi;
    //delete kSPC;
    delete kHadlock;
}

void Controller::start() {
    //kHadlock->start(kMap->get_routes());
    // This is the initial hanan grid
    //kEdges = kHadlock->get_edges();
    /**
    * Paper 5 details a multi-step process to generate the OAVG
    *
    * 1) Generate the Voronoi Diagram (vertices become steiner points)
    * 2) add steiner points
    * 3) use Dijkstra's/Kruskal's algorithm to pick the best steiner points
    */
    // Step 1
    kVoronoi->start();
    // add the nodes to our vertices

    add_to_all_vertices(kVoronoi->get_vertices());
    claim("C/start: Number of vertices from voronoi: " + to_string(kVoronoi->get_vertices().size()), kDebug);
    // Combine the voronoie with the pins
    //add_to_all_edges(kVoronoi->get_routes());

    // step 2
    kSteiner = new Steiner(kMap, kSteinerCalculator);
    kSteiner->start();
    set<VNode*> sp = kSteiner->get_steiner_points();
    claim("C/start: there are " + to_string(sp.size()) + " steiner points", kDebug);

    // Route all the MapRoutes that need to be routed
    kHadlock->start(kMap->get_routes());
    kHadlock->start(kVoronoi->get_routes());
    //kHadlock->start(kSteiner->get_routes());

    // Step 3
    // kEdges will have all the routeable edges possible
    this->kSPC = new SPC(kHadlock->get_edges());
    this->kSPC->start();

    kMap->print_map();
}

/**
* debugging for printing the map to the screen
*/
void Controller::print_map() {
    this->kMap->print_map();
}

/*
* alter the state of the map object given the edges
*/
void Controller::project_edges_on_map(set<VEdge *> e) {
    for(VEdge* v : e) {
        kMap->set(v->kStart);
        kMap->set(v->kEnd);
    }
}

/**
* alter the state of the map object given the vnodes
*/
void Controller::project_vertices_on_map(set<VNode *> n) {
    for(VNode* v : n) {
        kMap->set(v);
    }
}

int Controller::calculate_distance(int x, int y) {
    return (abs(x - 0) + abs(y - 0));
}

/**
* Calculating the manhattan distance of two nodes
*/
int Controller::calculate_manhattan_distance(VNode* a, VNode* b) {
    int order1, order2;
    order1 = abs(a->get_x() - b->get_x());
    order2 = abs(a->get_y() - b->get_y());
    return order1 + order2;
}

double Controller::calculate_euclidean_distance(VNode* a, VNode* b) {
    int order1, order2;
    order1 = (a->get_x() - b->get_x()) * (a->get_x() - b->get_x());
    order2 = (a->get_y() - b->get_y()) * (a->get_y() - b->get_y());
    // http://stackoverflow.com/questions/9695329/c-how-to-round-a-double-to-an-int
    return (sqrt(order1 + order2) + .5);
}

void Controller::set_steiner_calculator(int x) {
    kSteinerCalculator = x;
}

void Controller::add_to_all_vertices(set<VNode*> nodes) {
    for(auto node : nodes) {
        kVertices.insert(node);
    }
}

void Controller::add_to_all_edges(set<VEdge*> edges) {
    for(auto edge : edges) {
        kEdges.insert(edge);
    }
}

/*void Controller::add_to_all_routes(set<MapRoute*> routes) {
    for(auto r : routes) {
        kRoutes.insert(r);
    }
}*/