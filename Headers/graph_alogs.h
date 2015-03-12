#ifndef _FINAL_GRAPH_ALOGS_H_
#define _FINAL_GRAPH_ALOGS_H_

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

    class GraphAlgos {



    public:

        enum PHASE {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX};
        enum GRAPH_ALGO{DIJKSTRA, KRUSKAL};

        GraphAlgos();

        GraphAlgos(std::set<VEdge *>);

        ~GraphAlgos();

        // Templated entry point for all algorithmic starts
        void start();

    private:

        void run_dijkstra();

        void run_kruskal();

        void generate_dot_file(GRAPH_ALGO);
        void debug_graph_algorithm(GRAPH_ALGO);

        //GAGraph kGraph;
        std::vector<int> kD;
        //std::vector<GAVertex> kVD;

        std::set<VEdge*> kInputEdges;
        std::unordered_map<int, VNode*> kHashMap;
        std::string kGraphAlgo[2] = {"Dijkstra", "Kruskal"};
    };
}



#endif //_FINAL_GRAPH_ALOGS_H_
