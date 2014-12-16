#include "../Headers/flow_algo.h"
#include "../Headers/kruskal.h"
#include "../Headers/problem_object.h"
#include <string>
#include "../Headers/claim.h"
#include "../Headers/binary_tree.h"
#include "../Headers/vevent.h"

using namespace Utilities;
using namespace Flow;
using namespace std;

FlowAlgorithm::FlowAlgorithm() {
    kTree = BinaryTree();
}

FlowAlgorithm::~FlowAlgorithm() {
}

void FlowAlgorithm::start(priority_queue<VNode*, vector<VNode*>, CloserToOrigin> k) {

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