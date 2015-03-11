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
    VEdgeWrapper ve;
    for(VEdge* e : edges) {
        ve.id = e->kId;
        ve.weight = e->kCost;
        ve.source = e->kStart->get_id();
        ve.target = e->kEnd->get_id();
        kEdges.push_back(ve);
    }

    kNumNodes = (int)edges.size();

}

SPC::~SPC(){
    //delete kDijkstra;
}

void SPC::start() {
    claim("S/start: starting SPC", kDebug);
    run_dijkstra();
    run_kruskal();
    run_dijkstra_two(kEdges);
}


void SPC::run_dijkstra() {
    claim("S/run_dijkstra: starting boost/dijkstra", kDebug);

   /* DEdge kEdgeArray[] = {};
    int kWeightsArray[] = {};

    const int num_nodes = 5;
    enum nodes { A, B, C, D, E };
    char name[] = "ABCDE";

    DEdge edge_array[] = { DEdge(A, C), DEdge(B, B), DEdge(B, D), DEdge(B, E),
            DEdge(C, B), DEdge(C, D), DEdge(D, E), DEdge(E, A), DEdge(E, B)
    };

    int weights[] = { 1, 2, 1, 2, 7, 3, 1, 1, 1 };
    int num_arcs = sizeof(edge_array) / sizeof(DEdge);
    dijkstra_graph_t graph(edge_array, edge_array + num_arcs, weights, num_nodes);

    // Boost graph object
    property_map<dijkstra_graph_t, edge_weight_t>::type weightmap = get(edge_weight, graph);
    std::vector<dijkstra_vertex_descriptor> p(num_vertices(graph));
    std::vector<int> d(num_vertices(graph));
    dijkstra_vertex_descriptor s = vertex(A, graph);

    dijkstra_shortest_paths(graph, s,
            predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, graph))).
                    distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, graph))));

    std::cout << "distances and parents:" << std::endl;
    graph_traits < dijkstra_graph_t >::vertex_iterator vi, vend;
    for (boost::tie(vi, vend) = vertices(graph); vi != vend; ++vi) {
        std::cout << "distance(" << name[*vi] << ") = " << d[*vi] << ", ";
        std::cout << "parent(" << name[*vi] << ") = " << name[p[*vi]] << std::
        endl;
    }
    std::cout << std::endl;*/
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

void SPC::run_dijkstra_two(vector<VEdgeWrapper> edges) {
    using namespace boost::adaptors;

    size_t max_node;

    boost::partial_sort_copy(
            edges | transformed([](VEdgeWrapper const &e) -> size_t { return std::max(e.source, e.target); }),
            boost::make_iterator_range(&max_node, &max_node + 1),
            std::greater<size_t>());

    auto e = edges | transformed([](VEdgeWrapper const &ve) { return std::make_pair(ve.source, ve.target); });
    kGraph = dijkstra_graph_t(e.begin(), e.end(), edges.begin(), max_node + 1);

    weight_map_t kWeightMap = boost::get(&VEdgeWrapper::weight, kGraph);

    vertex_descriptor kS    = vertex(0, kGraph);
    kP                      = std::vector<vertex_descriptor>(num_vertices(kGraph));
    kD                      = std::vector<int>(num_vertices(kGraph));

    dijkstra_shortest_paths(
            kGraph, kS,
            predecessor_map(boost::make_iterator_property_map(kP.begin(), get(boost::vertex_index, kGraph)))
                    .distance_map(boost::make_iterator_property_map(kD.begin(), get(boost::vertex_index, kGraph)))
                    .weight_map(kWeightMap));

    std::cout << "distances and parents:" << std::endl;
    boost::graph_traits<dijkstra_graph_t>::vertex_iterator vi, vend;

    for (boost::tie(vi, vend) = vertices(kGraph); vi != vend; ++vi) {
        std::cout << "distance(" << *vi << ") = " << kD[*vi] << ", ";
        std::cout << "parent(" << *vi << ") = " << kP[*vi] << "\n";
    }

    /**
    * Generating the DOT file
    */
    std::ofstream dot_file("dijkstra-eg.dot",std::ofstream::out);

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
    for (boost::tie(ei, ei_end) = boost::edges(kGraph); ei != ei_end; ++ei) {
        boost::graph_traits<dijkstra_graph_t>::edge_descriptor e = *ei;
        boost::graph_traits<dijkstra_graph_t>::vertex_descriptor u = source(e, kGraph), v = target(e, kGraph);
        dot_file << u << " -> " << v << "[label=\"" << get(kWeightMap, e) << "\"";
        output += to_string(u) + " -> " + to_string(v) + "[label=\"" + to_string(get(kWeightMap, e)) + "\"";

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