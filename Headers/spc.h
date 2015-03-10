#ifndef _SPC_H_
#define _SPC_H_

#pragma once

#include <boost/config.hpp>
#include "vedge.h"

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

using namespace boost;
using namespace Flow;

namespace FlowAlgorithms {

    /**
    * Dijkstra definitions
    */
    typedef adjacency_list < listS, vecS, directedS,
            no_property, property < edge_weight_t, int > > dijkstra_graph_t;
    typedef graph_traits < dijkstra_graph_t >::vertex_descriptor dijkstra_vertex_descriptor;
    typedef std::pair<int, int> DEdge;

    /**
    * Kruskal definitions
    */
    typedef adjacency_list < vecS, vecS, undirectedS,
            no_property, property < edge_weight_t, int > > kruskal_graph_t;
    typedef graph_traits < kruskal_graph_t >::edge_descriptor kruskal_edge_desrciptor;
    typedef graph_traits < kruskal_graph_t >::vertex_descriptor kruskal_vertex_descriptor;
    typedef std::pair<int, int> kruskal_edge;

    class SPC {
    public:
        SPC();

        SPC(std::set<VEdge *>);

        ~SPC();

        // Templated entry point for all algorithmic starts
        void start();
        void start(std::set<VEdge*>);

    private:
        void run_dijkstra();
        void run_dijkstra_two();

        void run_kruskal();

        int kNumNodes;
        int* kWeightsArray;
        std::set<VEdge *> kEdges;
        std::vector<int> kWeights;
    };
}

#endif