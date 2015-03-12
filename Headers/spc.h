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
        Flow::VEdge* edge;
    };

    /**
    * Kruskal definitions
    */
    /*typedef adjacency_list < vecS, vecS, undirectedS,
            no_property, property < edge_weight_t, int > > kruskal_graph_t;
    typedef graph_traits < kruskal_graph_t >::edge_descriptor kruskal_edge_desrciptor;
    typedef graph_traits < kruskal_graph_t >::vertex_descriptor kruskal_vertex_descriptor;
    typedef std::pair<int, int> kruskal_edge;*/

    class SPC {
        /**
        * Dijkstra definitions
        */
        using dijkstra_graph_t  = boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, boost::no_property, VEdgeWrapper>;
        using vertex_descriptor = boost::graph_traits<dijkstra_graph_t>::vertex_descriptor;
        using edge_descriptor   = boost::graph_traits<dijkstra_graph_t>::edge_descriptor;
        using weight_map_t      = boost::property_map<dijkstra_graph_t, double VEdgeWrapper::*>::type;
        using d_start_map_t     = boost::property_map<dijkstra_graph_t, int VEdgeWrapper::*>::type;
        //using d_vedge_map_t     = boost::property_map<dijkstra_graph_t, VEdge VEdgeWrapper::*>type;

        using kruskal_graph_t       = boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, boost::no_property, VEdgeWrapper>;
        using k_vertex_descriptor   = boost::graph_traits<kruskal_graph_t>::vertex_descriptor;
        using k_edge_descriptor     = boost::graph_traits<kruskal_graph_t>::edge_descriptor;
        using k_weight_map_t        = boost::property_map<kruskal_graph_t, double VEdgeWrapper::*>::type;

    public:
        enum MapAlgorithm {DIJKSTRA, KRUSKAL};

        SPC();

        SPC(std::set<VEdge *>);

        ~SPC();

        // Templated entry point for all algorithmic starts
        void start();

    private:
        void run_dijkstra();

        void run_kruskal();

        void generate_dot_file(MapAlgorithm);
        void print_path(MapAlgorithm);
        void print_dijkstra_output();

        dijkstra_graph_t kDijkstraGraph;
        std::vector<int> kD;
        std::vector<vertex_descriptor> kP;
        std::vector<VEdgeWrapper> kInputEdges;
        std::vector<VEdge*> kDijkstraOutputEdges;
        std::unordered_map<int, VNode*> kHashMap;
    };
}

#endif