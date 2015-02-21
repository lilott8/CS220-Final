#include "../Headers/prim.h"
#include "../Headers/vedge.h"
#include "../Headers/vnode.h"
#include "../Headers/claim.h"

using namespace Algorithms;
using namespace Flow;

Prim::Prim() {

}

Prim::Prim(vector<VEdge*> edges) {
    kEdges = edges;
}

Prim::~Prim() {

}

void Prim::start() {
    claim("P/start: starting prims", kDebug);
    build_graph();
}


void Prim::build_graph() {

}