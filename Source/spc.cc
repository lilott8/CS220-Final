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
#include "graph_alogs.h"
#include "../Headers/controller.h"

using namespace FlowAlgorithms;
using namespace boost;
using namespace Flow;
using namespace boost::adaptors;

SPC::SPC(){}

SPC::SPC(std::set<VEdge*> edges) {
    int x = 0;
    for(VEdge* e : edges) {
        kInputEdges.push_back({ e->kId, e->kStart->get_id(), e->kEnd->get_id(), (double)e->kCost});
        // Save the nodes in a hashmaph for later use!
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
                run_dijkstra(kInputEdges);
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

    typedef adjacency_list<vecS, vecS, undirectedS, no_property,
            property<edge_weight_t, int> > GAGraph;

    typedef graph_traits<GAGraph>::edge_descriptor GAEdge;
    typedef graph_traits<GAGraph>::vertex_descriptor GAVertex;

    GAGraph g(kInputEdges.size());

    boost::property_map<GAGraph, boost::edge_weight_t>::type weightmap = boost::get(boost::edge_weight, g);
    std::vector<GAEdge> spanning_tree;

    int x = 0;
    for(VEdgeWrapper vw : kDijkstraEdges) {
        GAEdge gae; bool s;
        tie(gae, s) = add_edge(vw.source, vw.target, g);
        weightmap[gae] = vw.weight;
    }

    kruskal_minimum_spanning_tree(g, back_inserter(spanning_tree));

    std::string d_p = "S/run_kruskal:\nPrint the edges in the MST:";
    for (std::vector < GAEdge >::iterator ei = spanning_tree.begin();
         ei != spanning_tree.end(); ++ei) {
        d_p += to_string(source(*ei, g)) + " <--> " + to_string(target(*ei, g)) +
                + " with weight of " + to_string(weightmap[*ei]) + "\n";
    }
    //claim(d_p, kDebug);

    std::ofstream fout("figs/kruskal-eg.dot");
    string output = "S/run_kruskal:Kruskal:\ngraph A {\n";
    output += " rankdir=LR\n";
    output += " size=\"3,3\"\n";
    output += " ratio=\"filled\"\n";
    output += " edge[style=\"bold\"]\n node[shape=\"circle\"]\n";

    fout << "graph A {\n"
            << " rankdir=LR\n"
            << " size=\"3,3\"\n"
            << " ratio=\"filled\"\n"
            << " edge[style=\"bold\"]\n" << " node[shape=\"circle\"]\n";
    graph_traits<GAGraph>::edge_iterator eiter, eiter_end;
    for (boost::tie(eiter, eiter_end) = edges(g); eiter != eiter_end; ++eiter) {
        fout << source(*eiter, g) << " -- " << target(*eiter, g);
        output += to_string(source(*eiter, g)) + " -- " + to_string(target(*eiter, g));
        if (std::find(spanning_tree.begin(), spanning_tree.end(), *eiter)
                != spanning_tree.end()) {
            output += "[color=\"black\", label=\"" + to_string(get(edge_weight, g, *eiter)) + "\"];\n";
            fout << "[color=\"black\", label=\"" << get(edge_weight, g, *eiter)
                    << "\"];\n";
        } else {
            output += "[color=\"gray\", label=\"" + to_string(get(edge_weight, g, *eiter)) + "\"];\n";
            fout << "[color=\"gray\", label=\"" << get(edge_weight, g, *eiter)
                    << "\"];\n";
        }
    }
    fout << "}\n";
    output += "}\n";
    claim(output, kDebug);
    claim("S/run_kruskal: =========================================", kDebug);
}


void SPC::run_dijkstra(std::vector<VEdgeWrapper> edges) {
    claim("S/run_dijkstra: starting boost/dijkstra", kDebug);

    GRAPH_ALGO a = DIJKSTRA;

    std::vector<VEdgeWrapper> result;

    using namespace boost::adaptors;

    size_t max_node;

    boost::partial_sort_copy(
            edges | transformed([](VEdgeWrapper const &e) -> size_t { return std::max(e.source, e.target); }),
            boost::make_iterator_range(&max_node, &max_node + 1),
            std::greater<size_t>());

    auto e = edges | transformed([](VEdgeWrapper const &ve) { return std::make_pair(ve.source, ve.target); });
    kGraph = graph_t(e.begin(), e.end(), edges.begin(), max_node + 1);

    weight_map_t kWeightMap = boost::get(&VEdgeWrapper::weight, kGraph);

    vertex_descriptor kS    = vertex(0, kGraph);
    kVD                     = std::vector<vertex_descriptor>(num_vertices(kGraph));
    kD                      = std::vector<int>(num_vertices(kGraph));

    dijkstra_shortest_paths(
            kGraph, kS,
            predecessor_map(boost::make_iterator_property_map(kVD.begin(), get(boost::vertex_index, kGraph)))
                    .distance_map(boost::make_iterator_property_map(kD.begin(), get(boost::vertex_index, kGraph)))
                    .weight_map(kWeightMap));

    boost::graph_traits<graph_t>::edge_iterator ei, ei_end;

    /*for (boost::tie(ei, ei_end) = boost::edges(kGraph); ei != ei_end; ++ei) {
        boost::graph_traits<graph_t>::edge_descriptor e = *ei;
        boost::graph_traits<graph_t>::vertex_descriptor u = source(e, kGraph), v = target(e, kGraph);

        // Add the edges of the graph to the output, to prepare for Kruskal

    }*/

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

    //boost::graph_traits<graph_t>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(kGraph); ei != ei_end; ++ei) {
        boost::graph_traits<graph_t>::edge_descriptor e = *ei;
        boost::graph_traits<graph_t>::vertex_descriptor u = source(e, kGraph), v = target(e, kGraph);

        dot_file << u << " -> " << v << "[label=\"" << get(weight_map, e) << "\"";
        output += to_string(u) + " -> " + to_string(v) + "[label=\"" + to_string(get(weight_map, e)) + "\"";

        if (kVD[v] == u) {
            dot_file << ", color=\"black\"";
            output += ", color=\"black\"";

            VEdgeWrapper vw;
            vw.id = VEdge::get_next_vedge_id();
            vw.source = u;
            vw.target = v;
            vw.weight = Flow::Controller::calculate_manhattan_distance(kHashMap.at((int) u), kHashMap.at((int) v));
            kDijkstraEdges.push_back(vw);

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

    claim("S/run_dijkstra: =========================================", kDebug);
}