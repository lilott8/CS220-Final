#ifndef _PRIM_H_
#define _PRIM_H_

#include "vedge.h"

#pragma once

using namespace Flow;

namespace Algorithms {

    //typedef adjacency_list < vecS, vecS, undirectedS,
    //        property<vertex_distance_t, int>, property < edge_weight_t, int > > Graph;
    //typedef std::pair < int, int >E;

    class Prim {
    public:
        Prim();
        Prim(vector<VEdge*>);
        ~Prim();
        // Templated entry point for all algorithmic starts
        void start();

    private:
        //Graph kGraph;
        vector<VEdge*> kEdges;

        void build_graph();
    };
}

#endif