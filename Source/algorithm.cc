#include "../Headers/algorithm.h"
#include "../Headers/kruskal.h"
#include "../Headers/problem_object.h"
#include <string>
#include "../Headers/claim.h"

using namespace Utilities;
using namespace Flow;
using namespace std;

Algorithm::Algorithm() {
}

Algorithm::~Algorithm() {
}

void Algorithm::start() {
    claim("A/Start: Starting the algorithm", kDebug);
}

string Algorithm::print_algo_type(AlgoType t) {
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

string Algorithm::print_optimization(Optimization t) {
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