#ifndef _SPC_H_
#define _SPC_H_

#pragma once

#include "btree.h"
#include "vnode.h"
#include "flow_algo.h"
#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <vector>

using namespace Utilities;
using namespace Flow;
using namespace boost;
using namespace std;

namespace Algorithms {
    class SPC {

        typedef adjacency_list < listS, vecS, directedS,
                no_property, property < edge_weight_t, int > > graph_t;
        typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
        typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;

    public:
        SPC();
        SPC(vector<VEdge*>);
        ~SPC();

        // Templated entry point for all algorithmic starts
        void start();
    private:
        void run_dijkstra();
        void run_kruskal();

        int kNumNodes;
        vector<VEdge*> kEdges;
        vector<int> kWeights;
        graph_t kGraph;
    };
}

#endif