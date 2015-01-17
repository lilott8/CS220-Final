#include <string>
#include <queue>
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
    // Clear the lists first!
    clear_all();

    while(!k.empty()) {
        kPins.push_back(k.top());
        k.pop();
    }

    claim("A/Start: Starting the algorithm", kDebug);
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

void FlowAlgorithm::set_map_size(double maxx, double maxy, double minx, double miny) {
    kMaxHeight = maxx;
    kMaxWidth = maxy;
    kMinWidth = miny;
    kMinHeight = minx;
    claim("FA/set_map_size: max_height: " + to_string(maxx), kDebug);
    claim("FA/set_map_size: max_width: " + to_string(maxy), kDebug);
    claim("FA/set_map_size: min_height: " + to_string(minx), kDebug);
    claim("FA/set_map_size: min_width: " + to_string(miny), kDebug);
}

void FlowAlgorithm::clear_all() {
    for(vector<VNode*>::iterator i = kPins.begin(); i != kPins.end(); i++) {
        delete(*i);
    }
    kPins.clear();
    kEdges.clear();
}