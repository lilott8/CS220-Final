#include <string>
#include "../Headers/flow_algo.h"
#include "../Headers/kruskal.h"
#include "../Headers/problem_object.h"
#include "../Headers/claim.h"

using namespace Utilities;
using namespace Flow;
using namespace std;

FlowAlgorithm::FlowAlgorithm() {
}

FlowAlgorithm::~FlowAlgorithm() {
    clear_all();
}

void FlowAlgorithm::start(priority_queue<VNode*, vector<VNode*>, CloserToOrigin> k) {

    claim("A/Start: Starting the algorithm", kDebug);
    clear_all();
}

string FlowAlgorithm::print_algo_type(AlgoType t) {
    string s = "";
    switch(t) {
        case KRUSKAL:
            s = "kruskal";
            break;
        default:
        case FORTUNE:
            s = "fortune";
            break;
        case SPM:
            s = "spm";
            break;
    }
    return s;
}

string FlowAlgorithm::print_optimization(Optimization t) {
    string s = "";
    switch (t) {
        case H_OPT:
            s = "h_opt";
            break;
        case U_OPT:
            s = "u_opt";
            break;
        default:
        case DEFAULT:
            s = "default";
            break;
    }
    return s;
}

void FlowAlgorithm::set_map_size(double x, double y) {
    kHeight = x;
    kWidth = y;
}

void FlowAlgorithm::clear_all() {
    for(list<VNode*>::iterator i = kPoints.begin(); i != kPoints.end(); ++i) {
        delete (*i);
    }
    for(list<VNode*>::iterator i = kPlaces.begin(); i != kPlaces.end(); ++i) {
        delete (*i);
    }

    kPlaces.clear();
    kEdges.clear();
    kPoints.clear();
}