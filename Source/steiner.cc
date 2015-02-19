#include "../Headers/map.h"
#include "../Headers/steiner.h"

using namespace Flow;
using namespace Algorithms;

Steiner::Steiner() {
}

Steiner::~Steiner() {
}


void Steiner::start() {
    claim("Size of queue: " + to_string(kAllVertices.size()), kDebug);
    for(int x = 0;x<kAllVertices.size();x++) {
        for(int y = x+1;y<kAllVertices.size();y++) {
            for(int z = y+1;z<kAllVertices.size();z++) {
                if((kAllVertices.at(x)->get_x() == kAllVertices.at(y)->get_x()
                                && kAllVertices.at(x)->get_y() == kAllVertices.at(y)->get_y()) ||
                        (kAllVertices.at(x)->get_x() == kAllVertices.at(z)->get_x()
                                && kAllVertices.at(x)->get_y() == kAllVertices.at(z)->get_y()) ||
                        (kAllVertices.at(y)->get_x() == kAllVertices.at(z)->get_x()
                                && kAllVertices.at(y)->get_y() == kAllVertices.at(z)->get_y())) {
                    //claim("Skipping the equal vertices", kDebug);
                    //continue;
                }
                //claim(kAllVertices.at(x)->vnode_to_string() + "\t" + kAllVertices.at(y)->vnode_to_string() + "\t" + kAllVertices.at(z)->vnode_to_string(), kDebug);
            }
        }
    }
}

void Steiner::set_vertices(vector<VNode*> v) {
    kAllVertices = v;
    for(VNode* x : v) {
        kBinaryTree.insert(x);
        //kPQueue.push(x);
    }
}