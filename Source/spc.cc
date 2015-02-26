#include "../Headers/spc.h"
#include "../Headers/btree.h"
#include <vector>
#include <boost/polygon/voronoi_diagram.hpp>
#include <claim.h>

using namespace Algorithms;
using namespace Flow;

SPC::SPC(){}

SPC::SPC(vector<VEdge*> edges) {
    kEdges = edges;
    kNumNodes = (int)edges.size();
}

SPC::~SPC(){}

void SPC::start() {
    claim("S/start: starting SPC", kDebug);
    run_dijkstra();
    run_kruskal();
}

void SPC::run_dijkstra() {
    claim("S/run_dijkstra: starting boost/dijkstra", kDebug);

    /*
    kGraph = graph_t(edge_array, edge_array + num_args, kWeights, kNumNodes);
    //graph_t g(edge_array, edge_array + num_arcs, kWeights, kNumNodes);
    property_map<graph_t, edge_weight_t>::type weight_map = get(edge_weight, kGraph);

    vector<vertex_descriptor> p(num_vertices(kGraph));
    vector<int> d(num_vertices(kGraph));
    vertex_descriptor v_d = vertex(A, kGraph);

    dijkstra_shortest_paths(kGraph, v_d, predecessor_map(&p[0]).distance_map(&d[0]));
    */
}

void SPC::run_kruskal() {
    claim("S/run)kruskal: starting boost/kruskal", kDebug);
}