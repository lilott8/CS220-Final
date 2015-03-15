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

    public:

        enum PHASE {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX};
        enum GRAPH_ALGO{DIJKSTRA, KRUSKAL};

        SPC();

        SPC(std::set<VEdge *>);

        ~SPC();

        // Templated entry point for all algorithmic starts
        void start();

    private:

        void run_dijkstra(std::vector<VEdgeWrapper>);

        void run_kruskal();

        void generate_dot_file(GRAPH_ALGO);
        void debug_graph_algorithm(GRAPH_ALGO);

        graph_t kGraph;
        std::vector<int> kD;
        std::vector<vertex_descriptor> kVD;

        // Inputted by elsewhere
        std::vector<VEdgeWrapper> kInputEdges;
        // output of dijkstra's algorithm
        std::vector<VEdgeWrapper> kDijkstraEdges;
        // ouptut of kruskal's algorithm
        std::vector<VEdgeWrapper> kKruskalEdges;
        // Map of SPC values to their actual node implimentation
        std::unordered_map<int, VNode*> kHashMap;
        std::string kGraphAlgo[2] = {"Dijkstra", "Kruskal"};
    };
}

#endif