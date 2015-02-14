#include "../Headers/map.h"
#include "../Headers/steiner.h"

using namespace Flow;
using namespace Algorithms;

Steiner::Steiner() {
}

Steiner::~Steiner() {
}


void Steiner::start() {
}

void Steiner::set_vertices(vector<VNode*> v) {
    kAllVertices = v;
    for(VNode* x : v) {
        kBinaryTree.insert(x);
        //kPQueue.push(x);
    }
}