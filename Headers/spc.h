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
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/property_map/transform_value_property_map.hpp>
#include <unordered_map>

using namespace boost;
using namespace Flow;

namespace FlowAlgorithms {

    struct VEdgeWrapper {
        int id;
        int source, target;
        double weight;
        //Flow::VEdge* edge;
    };

    class SPC {
        /**
        * Dijkstra definitions
        */
        using graph_t  = boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, boost::no_property, VEdgeWrapper>;
        using vertex_descriptor = boost::graph_traits<graph_t>::vertex_descriptor;
        using edge_descriptor   = boost::graph_traits<graph_t>::edge_descriptor;
        using weight_map_t      = boost::property_map<graph_t, double VEdgeWrapper::*>::type;

        /**
        * Kruskal definitions
        */
        //using kruskal_graph_t       = boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, boost::no_property, VEdgeWrapper>;
        //using k_vertex_descriptor   = boost::graph_traits<kruskal_graph_t>::vertex_descriptor;
        //using k_edge_descriptor     = boost::graph_traits<kruskal_graph_t>::edge_descriptor;
        //using k_weight_map_t        = boost::property_map<kruskal_graph_t, double VEdgeWrapper::*>::type;

    public:

        enum PHASE {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX};
        enum GRAPH_ALGO{DIJKSTRA, KRUSKAL};

        SPC();

        SPC(std::set<VEdge *>);

        ~SPC();

        // Templated entry point for all algorithmic starts
        void start();

    private:

        void run_dijkstra();

        void run_kruskal();

        void generate_dot_file(GRAPH_ALGO);
        void debug_graph_algorithm(GRAPH_ALGO);
        void print_dijkstra_output();

        graph_t kGraph;
        //kruskal_graph_t kKruskalGraph;
        std::vector<int> kD;
        std::vector<vertex_descriptor> kVD;
        //std::vector<k_vertex_descriptor> kKruskalVD;

        std::vector<VEdgeWrapper> kInputEdges;
        std::vector<VEdge*> kDijkstraOutputEdges;
        std::unordered_map<int, VNode*> kHashMap;
        std::string kGraphAlgo[2] = {"Dijkstra", "Kruskal"};
    };
}

#endif