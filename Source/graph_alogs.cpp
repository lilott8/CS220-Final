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

GraphAlgos::GraphAlgos(){}

GraphAlgos::GraphAlgos(std::set<VEdge*> edges) {
kInputEdges = edges;
}

GraphAlgos::~GraphAlgos(){}

void GraphAlgos::start() {
    claim("S/start: starting GraphAlgos", kDebug);

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

void GraphAlgos::run_kruskal() {
    claim("S/run_kruskal: starting boost/kruskal", kDebug);

    typedef adjacency_list<vecS, vecS, undirectedS, no_property,
            property<edge_weight_t, int> > GAGraph;

    typedef graph_traits<GAGraph>::edge_descriptor GAEdge;
    typedef graph_traits<GAGraph>::vertex_descriptor GAVertex;

    GAGraph g(kInputEdges.size());

    boost::property_map<GAGraph, boost::edge_weight_t>::type weightmap = boost::get(boost::edge_weight, g);
    std::vector<GAEdge> spanning_tree;

    int x = 0;
    for(VEdge* vw : kInputEdges) {
        GAEdge gae; bool s;
        tie(gae, s) = add_edge(vw->kStart->get_id(), vw->kEnd->get_id(), g);
        weightmap[gae] = vw->kCost;
    }

    kruskal_minimum_spanning_tree(g, back_inserter(spanning_tree));

    std::cout << "Print the edges in the MST:" << std::endl;
    for (std::vector < GAEdge >::iterator ei = spanning_tree.begin();
         ei != spanning_tree.end(); ++ei) {
        std::cout << source(*ei, g) << " <--> " << target(*ei, g)
                << " with weight of " << weightmap[*ei]
                << std::endl;
    }

    std::ofstream fout("figs/kruskal-eg.dot");
    string output = "graph A {\n";
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
}


void GraphAlgos::run_dijkstra() {
    claim("S/run_dijkstra: starting boost/dijkstra", kDebug);

    typedef adjacency_list < listS, vecS, directedS,
            no_property, property < edge_weight_t, int > > GAGraph;
    typedef graph_traits < GAGraph >::vertex_descriptor GAVertex;
    typedef std::pair<int, int> GAEdge;

    int num_nodes = (int)kInputEdges.size();

    GAEdge* edge_array = new GAEdge[num_nodes];
    int* weights = new int[num_nodes];

    int x = 0;
    for(VEdge* e : kInputEdges) {
        edge_array[x] = GAEdge(e->kStart->get_id(), e->kEnd->get_id());
        weights[x] = e->kCost;
        x++;
    }

    int num_arcs = sizeof(edge_array) / sizeof(GAEdge);
    GAGraph g(edge_array, edge_array + num_arcs, weights, num_nodes);

    property_map<GAGraph, edge_weight_t>::type weightmap = get(edge_weight, g);
    std::vector<GAVertex> p(num_vertices(g));
    std::vector<int> d(num_vertices(g));
    GAVertex s = vertex(0, g);

    dijkstra_shortest_paths(g, s,
            predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
                    distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));

    /**
    * OUTPUT
    */
    std::cout << "distances and parents:" << std::endl;
    graph_traits < GAGraph >::vertex_iterator vi, vend;
    for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
        std::cout << "distance(" << *vi << ") = " << d[*vi] << ", ";
        std::cout << "parent(" << *vi << ") = " << p[*vi] << std::
        endl;
    }
    std::cout << std::endl;

    std::ofstream dot_file("figs/dijkstra-eg.dot");

    dot_file << "digraph D {\n"
            << "  rankdir=LR\n"
            << "  size=\"4,3\"\n"
            << "  ratio=\"fill\"\n"
            << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

    graph_traits < GAGraph >::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
        graph_traits < GAGraph >::edge_descriptor e = *ei;
        graph_traits < GAGraph >::vertex_descriptor u = source(e, g), v = target(e, g);
        dot_file << u << " -> " << v
                << "[label=\"" << get(weightmap, e) << "\"";
        if (p[v] == u)
            dot_file << ", color=\"black\"";
        else
            dot_file << ", color=\"grey\"";
        dot_file << "]";
    }
    dot_file << "}";



    delete[] edge_array;
    delete[] weights;
}