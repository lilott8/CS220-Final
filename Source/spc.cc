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
        kInputEdges.push_back({ e->kId, e->kStart->get_id(), e->kEnd->get_id(), (double)e->kCost});//, e});
        kHashMap.insert({e->kStart->get_id(), e->kStart});
        kHashMap.insert({e->kEnd->get_id(), e->kEnd});
    }
}

SPC::~SPC(){}

void SPC::start() {
    claim("S/start: starting SPC", kDebug);

    int num_steps = 5;
    vector<VEdgeWrapper> subset;

    for(int step = ONE; step != SIX; step++) {
        claim("Step " + to_string(step) + " of " + to_string(SIX), kDebug);
        switch(step) {
            // generate Dijkstra from subset of InputEdges (G1)
            case ONE:
                run_dijkstra();
                break;
                // generate Kruskal from Dijkstra outputs (G2)
            case TWO:
                run_kruskal();
                break;
                // replace edges in G2 with their shortest edge from G (G4)
            case THREE:

                break;
                // generate Kruskal from G4 (G5)
            case FOUR:

                break;
                // supposedely delete edges from G4, done!
            case FIVE:

                break;
                // noop
            default:
            case ZERO:

                break;
        }
    }
}

void SPC::run_kruskal() {
    claim("S/run_kruskal: starting boost/kruskal", kDebug);

    using namespace boost::adaptors;

    size_t max_node;

    boost::partial_sort_copy(
            kInputEdges | transformed([](VEdgeWrapper const &e) -> size_t { return std::max(e.source, e.target); }),
            boost::make_iterator_range(&max_node, &max_node + 1),
            std::greater<size_t>());

    auto e = kInputEdges | transformed([](VEdgeWrapper const &ve) { return std::make_pair(ve.source, ve.target); });
    kGraph = graph_t(e.begin(), e.end(), kInputEdges.begin(), max_node + 1);

    weight_map_t kWeightMap = boost::get(&VEdgeWrapper::weight, kGraph);

    vertex_descriptor kS  = vertex(0, kGraph);
    kVD              = std::vector<vertex_descriptor>(num_vertices(kGraph));
    kD                      = std::vector<int>(num_vertices(kGraph));

    dijkstra_shortest_paths(
            kGraph, kS,
            predecessor_map(boost::make_iterator_property_map(kVD.begin(), get(boost::vertex_index, kGraph)))
                    .distance_map(boost::make_iterator_property_map(kD.begin(), get(boost::vertex_index, kGraph)))
                    .weight_map(kWeightMap));

    debug_graph_algorithm(KRUSKAL);
    generate_dot_file(KRUSKAL);
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
    kGraph = graph_t(e.begin(), e.end(), kInputEdges.begin(), max_node + 1);

    weight_map_t kWeightMap = boost::get(&VEdgeWrapper::weight, kGraph);

    vertex_descriptor kS    = vertex(0, kGraph);
    kVD             = std::vector<vertex_descriptor>(num_vertices(kGraph));
    kD                      = std::vector<int>(num_vertices(kGraph));

    dijkstra_shortest_paths(
            kGraph, kS,
            predecessor_map(boost::make_iterator_property_map(kVD.begin(), get(boost::vertex_index, kGraph)))
                    .distance_map(boost::make_iterator_property_map(kD.begin(), get(boost::vertex_index, kGraph)))
                    .weight_map(kWeightMap));

    debug_graph_algorithm(DIJKSTRA);
    generate_dot_file(DIJKSTRA);
    //print_dijkstra_output();
}

// TODO: figure out why the output values for distance are the max value of int...
void SPC::debug_graph_algorithm(GRAPH_ALGO a) {
    boost::graph_traits<graph_t>::vertex_iterator vi, vend;
    string output = kGraphAlgo[a] + ": distances and parents:\n";
    for (boost::tie(vi, vend) = vertices(kGraph); vi != vend; ++vi) {
        output += "distance(" + to_string(*vi) + ") = " + to_string(kD[*vi]) + ", ";
        output += "parent(" + to_string(*vi) + ") = " + to_string(kVD[*vi]) + "\n";
    }
    claim(output, kDebug);
}

void SPC::generate_dot_file(GRAPH_ALGO a) {
    std::string output_file_name;
    output_file_name = kGraphAlgo[a] + "-eg.dot";
    weight_map_t weight_map = boost::get(&VEdgeWrapper::weight, kGraph);

    std::ofstream dot_file(output_file_name,std::ofstream::out);

    string output = kGraphAlgo[a] + ":\ndigraph D {\nrankdir=LR\n";
    output += "  size=\"4,3\"\nratio=\"fill\"\n";
    output += "  edge[style=\"bold\"]\nnode[shape=\"circle\"]\n";

    dot_file << "digraph D {\n"
            << "  rankdir=LR\n"
            << "  size=\"4,3\"\n"
            << "  ratio=\"fill\"\n"
            << "  edge[style=\"bold\"]\n"
            << "  node[shape=\"circle\"]\n";

    boost::graph_traits<graph_t>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(kGraph); ei != ei_end; ++ei) {
        boost::graph_traits<graph_t>::edge_descriptor e = *ei;
        boost::graph_traits<graph_t>::vertex_descriptor u = source(e, kGraph), v = target(e, kGraph);
        dot_file << u << " -> " << v << "[label=\"" << get(weight_map, e) << "\"";
        output += to_string(u) + " -> " + to_string(v) + "[label=\"" + to_string(get(weight_map, e)) + "\"";

        // Add the edges of the graph to the output, to prepare for Kruskal
        //kDijkstraOutputEdges.push_back(new VEdge(kHashMap.at((int)u), kHashMap.at((int)v)));

        if (kVD[v] == u) {
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