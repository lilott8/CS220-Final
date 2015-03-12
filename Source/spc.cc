#include "../Headers/spc.h"
#include "../Headers/vedge.h"
#include "../headers/claim.h"
#include <boost/property_map/transform_value_property_map.hpp>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <fstream>
#include <iostream>

using namespace FlowAlgorithms;
using namespace boost;
using namespace Flow;
using namespace boost::adaptors;

SPC::SPC(){}

SPC::SPC(std::set<VEdge*> edges) {
    for(VEdge* e : edges) {
        kInputEdges.push_back({ e->kId, e->kStart->get_id(), e->kEnd->get_id(), (double)e->kCost, e});
        kHashMap.insert({e->kStart->get_id(), e->kStart});
        kHashMap.insert({e->kEnd->get_id(), e->kEnd});
    }
}

SPC::~SPC(){}

void SPC::start() {
    claim("S/start: starting SPC", kDebug);
    run_dijkstra();
    run_kruskal();
}

void SPC::run_kruskal() {
    claim("S/run_kruskal: starting boost/kruskal", kDebug);

    /*const int num_nodes = 5;
    kruskal_edge edge_array[] = { kruskal_edge(0, 2), kruskal_edge(1, 3), kruskal_edge(1, 4), kruskal_edge(2, 1), kruskal_edge(2, 3),
            kruskal_edge(3, 4), kruskal_edge(4, 0), kruskal_edge(4, 1)
    };
    int weights[] = { 1, 1, 2, 7, 3, 1, 1, 1 };
    std::size_t num_edges = sizeof(edge_array) / sizeof(kruskal_edge);
    kruskal_graph_t graph(edge_array, edge_array + num_edges, weights, num_nodes);

    property_map < kruskal_graph_t, edge_weight_t >::type weight = get(edge_weight, graph);
    std::vector < kruskal_edge > spanning_tree;

    kruskal_minimum_spanning_tree(graph, std::back_inserter(spanning_tree));

    std::cout << "Print the edges in the MST:" << std::endl;
    for (std::vector < kruskal_edge >::iterator ei = spanning_tree.begin();
         ei != spanning_tree.end(); ++ei) {
        std::cout << source(*ei, graph) << " <--> " << target(*ei, graph)
                << " with weight of " << weight[*ei]
                << std::endl;
    }*/
}

void SPC::run_dijkstra() {
    claim("S/run_dijkstra: starting boost/dijkstra", kDebug);

    using namespace boost::adaptors;

    size_t max_node;

    boost::partial_sort_copy(
            kInputEdges | transformed([](VEdgeWrapper const &e) -> size_t { return std::max(e.source, e.target); }),
            boost::make_iterator_range(&max_node, &max_node + 1),
            std::greater<size_t>());

    auto e = kInputEdges | transformed([](VEdgeWrapper const &ve) { return std::make_pair(ve.source, ve.target); });
    kDijkstraGraph = dijkstra_graph_t(e.begin(), e.end(), kInputEdges.begin(), max_node + 1);

    weight_map_t kWeightMap = boost::get(&VEdgeWrapper::weight, kDijkstraGraph);

    vertex_descriptor kS    = vertex(1, kDijkstraGraph);
    kP                      = std::vector<vertex_descriptor>(num_vertices(kDijkstraGraph));
    kD                      = std::vector<int>(num_vertices(kDijkstraGraph));

    dijkstra_shortest_paths(
            kDijkstraGraph, kS,
            predecessor_map(boost::make_iterator_property_map(kP.begin(), get(boost::vertex_index, kDijkstraGraph)))
                    .distance_map(boost::make_iterator_property_map(kD.begin(), get(boost::vertex_index, kDijkstraGraph)))
                    .weight_map(kWeightMap));

    //print_path(DIJKSTRA);
    generate_dot_file(DIJKSTRA);
    print_dijkstra_output();
}

// TODO: figure out why the output values for distance are the max value of int...
void SPC::print_path(MapAlgorithm a) {
    std::cout << "distances and parents:" << std::endl;
    boost::graph_traits<dijkstra_graph_t>::vertex_iterator vi, vend;

    string output = "";

    for (boost::tie(vi, vend) = vertices(kDijkstraGraph); vi != vend; ++vi) {
        //std::cout << *vi << std::endl;
        output += "distance(" + to_string(*vi) + ") = " + to_string(kD[*vi]) + ", ";
        output += "parent(" + to_string(*vi) + ") = " + to_string(kP[*vi]) + "\n";
    }

    claim(output, kDebug);
}


void SPC::generate_dot_file(MapAlgorithm a) {

    std::string output_file_name;
    weight_map_t weight_map = boost::get(&VEdgeWrapper::weight, kDijkstraGraph);
    output_file_name = "dijkstra-eg.dot";

    std::ofstream dot_file(output_file_name,std::ofstream::out);

    string output = "digraph D {\nrankdir=LR\n";
    output += "  size=\"4,3\"\nratio=\"fill\"\n";
    output += "  edge[style=\"bold\"]\nnode[shape=\"circle\"]\n";

    dot_file << "digraph D {\n"
            << "  rankdir=LR\n"
            << "  size=\"4,3\"\n"
            << "  ratio=\"fill\"\n"
            << "  edge[style=\"bold\"]\n"
            << "  node[shape=\"circle\"]\n";

    boost::graph_traits<dijkstra_graph_t>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(kDijkstraGraph); ei != ei_end; ++ei) {
        boost::graph_traits<dijkstra_graph_t>::edge_descriptor e = *ei;
        boost::graph_traits<dijkstra_graph_t>::vertex_descriptor u = source(e, kDijkstraGraph), v = target(e, kDijkstraGraph);
        dot_file << u << " -> " << v << "[label=\"" << get(weight_map, e) << "\"";
        output += to_string(u) + " -> " + to_string(v) + "[label=\"" + to_string(get(weight_map, e)) + "\"";

        // Add the edges of the graph to the output, to prepare for Kruskal
        kDijkstraOutputEdges.push_back(new VEdge(kHashMap.at((int)u), kHashMap.at((int)v)));

        if (kP[v] == u) {
            dot_file << ", color=\"black\"";
            output += ", color=\"black\"";
        } else {
            dot_file << ", color=\"grey\"";
            output += ", color=\"grey\"";
        }
        dot_file << "]";
        output += "]";
    }
    dot_file << "}";
    output += "}";

    claim(output, kDebug);
    dot_file.close();
}

void SPC::print_dijkstra_output() {
    for(VEdge* e : kDijkstraOutputEdges) {
        claim(e->vedge_to_string(), kDebug);
    }
}