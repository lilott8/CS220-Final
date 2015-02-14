#ifndef _FORTUNE_H_
#define _FORTUNE_H_

#pragma once

#include "map.h"
#include "voronoi.h"
#include "btree.h"
#include <set>

using namespace Utilities;
using namespace std;
using namespace Flow;

namespace Algorithms {

    struct compare_distance {
        bool operator() (VNode i, VNode j) {
            return i.get_cost() < j.get_cost();
        }
    };

    class Steiner {
    public:
        Steiner();
        ~Steiner();
        // Run the algorithm, this will actually generate the edges and
        void start();

        void set_vertices(vector<VNode*>);

    private:
        vector<VNode*> kAllVertices;
        //priority_queue<VNode*, vector<VNode*>, Algorithms::compare_distance> kPQueue;

        BinaryTree kBinaryTree = BinaryTree();
    };
}
#endif