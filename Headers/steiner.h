#ifndef _FORTUNE_H_
#define _FORTUNE_H_

#pragma once

#include "map.h"
#include "voronoi.h"
#include "btree.h"
#include <math.h>

using namespace Utilities;
using namespace std;
using namespace Flow;

namespace FlowAlgorithms {
    class Steiner {
    public:
        const double PI=3.14159265358979323846;

        Steiner();
        Steiner(set<VNode*>);
        ~Steiner();
        // Templated entry point for all algorithmic starts
        void start();

        void set_vertices(set<VNode*>);
        set<VNode*> get_steiner_points();
        set<VEdge*> get_steiner_edges();

    private:
        set<VNode*> kAllVertices;
        set<VNode*> kSteinerPoints;
        set<VEdge*> kSteinerEdges;

        void generate_steiner_intersections();
        void generate_steiner_intersections_two();
    };
}
#endif