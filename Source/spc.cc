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

/**
* Start running the SPC paper
*/
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
                // noop
            default:
            case ZERO:

                break;
        }
    }
}

/**
* Run the boost version of kruskal.  This utilizes the output from the dijkstra's method as input
*/
void SPC::run_kruskal() {
    claim("S/run_kruskal: starting boost/kruskal", kDebug);

    /**
    * build the typdefs required for boost documentation
    */
    typedef adjacency_list<vecS, vecS, undirectedS, no_property,
            property<edge_weight_t, int> > GAGraph;

    typedef graph_traits<GAGraph>::edge_descriptor GAEdge;
    typedef graph_traits<GAGraph>::vertex_descriptor GAVertex;

    // generate a graph, allocating the adjancency_list of inputedges sizes.
    GAGraph g(kDijkstraEdges.size());

    // build the spanning_tree vector in which will hold the answer
    boost::property_map<GAGraph, boost::edge_weight_t>::type weightmap = boost::get(boost::edge_weight, g);
    std::vector<GAEdge> spanning_tree;

    // convert the kDijkstraEdges to the input required for the kruskal input
    int x = 0;
    for(VEdgeWrapper vw : kDijkstraEdges) {
        GAEdge gae; bool s;
        tie(gae, s) = add_edge(vw.source, vw.target, g);
        weightmap[gae] = vw.weight;
    }

    // run the algorithm based on the input
    kruskal_minimum_spanning_tree(g, back_inserter(spanning_tree));

    // start building the output for the MST
    std::string d_p = "S/run_kruskal:\nPrint the edges in the MST:";
    for (std::vector < GAEdge >::iterator ei = spanning_tree.begin();
         ei != spanning_tree.end(); ++ei) {
        d_p += to_string(source(*ei, g)) + " <--> " + to_string(target(*ei, g)) +
                + " with weight of " + to_string(weightmap[*ei]) + "\n";
    }
    //claim(d_p, kDebug);

    // Generate the graphviz output
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

        output += kHashMap.at(source(*eiter, g))->vnode_to_dot() + " -- "
                + kHashMap.at(target(*eiter, g))->vnode_to_dot();

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

/**
* run the boost implementation of dijkstra's algorithm
*/
void SPC::run_dijkstra(std::vector<VEdgeWrapper> edges) {
    claim("S/run_dijkstra: starting boost/dijkstra", kDebug);

    GRAPH_ALGO a = DIJKSTRA;

    std::vector<VEdgeWrapper> result;

    using namespace boost::adaptors;

    size_t max_node;

    // partially sort the list of our input edges, and get them ready for djikstra
    boost::partial_sort_copy(
            edges | transformed([](VEdgeWrapper const &e) -> size_t { return std::max(e.source, e.target); }),
            boost::make_iterator_range(&max_node, &max_node + 1),
            std::greater<size_t>());

    // I'm not going to lie, I don't know exactly what this line does
    auto e = edges | transformed([](VEdgeWrapper const &ve) { return std::make_pair(ve.source, ve.target); });
    // instantiate the graph object for dijkstra
    kGraph = graph_t(e.begin(), e.end(), edges.begin(), max_node + 1);

    weight_map_t kWeightMap = boost::get(&VEdgeWrapper::weight, kGraph);

    // get the itereators and answer containers
    vertex_descriptor kS    = vertex(0, kGraph);
    kVD                     = std::vector<vertex_descriptor>(num_vertices(kGraph));
    kD                      = std::vector<int>(num_vertices(kGraph));

    // run the actual dijkstra's algorithm on our input
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

    // build the output for the our answer (graphviz is included in this)
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
        output += kHashMap.at(u)->vnode_to_dot() + " -> " + kHashMap.at(v)->vnode_to_dot()
                + "[label=\"" + to_string(get(weight_map, e)) + "\"";

        // we only care about the "important" edges, the secondary routes are irrelevant to us
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
        dot_file << "]\n";
        output += "]\n";
    }
    dot_file << "}";
    output += "}";

    claim(output, kDebug);
    dot_file.close();

    claim("S/run_dijkstra: =========================================", kDebug);
}