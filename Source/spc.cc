#include "../Headers/spc.h"
#include "../Headers/vedge.h"
#include "../headers/claim.h"

using namespace FlowAlgorithms;
using namespace boost;
using namespace Flow;

SPC::SPC(){}

SPC::SPC(std::set<VEdge*> edges) {
    kEdges = edges;
    kNumNodes = (int)edges.size();
}

SPC::~SPC(){
    //delete kDijkstra;
}

void SPC::start() {
    claim("S/start: starting SPC", kDebug);
    run_dijkstra();
    run_kruskal();
}


void SPC::run_dijkstra() {
    claim("S/run_dijkstra: starting boost/dijkstra", kDebug);

    DEdge kEdgeArray[] = {};
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
    //int num_arcs = sizeof(kEdgeArray) / sizeof(DEdge);
    //dijkstra_graph_t graph(kEdgeArray, kEdgeArray + num_arcs, kWeights, kNumNodes);
    property_map<dijkstra_graph_t, edge_weight_t>::type weightmap = get(edge_weight, graph);
    std::vector<dijkstra_vertex_descriptor> p(num_vertices(graph));
    std::vector<int> d(num_vertices(graph));
    dijkstra_vertex_descriptor s = vertex(A, graph);
    //dijkstra_vertex_descriptor s = vertex(kEdgeArray[0].first, graph);

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
    std::cout << std::endl;



    // This generates an "arithmetic exception
    //int num_arcs = (int) sizeof(kEdgeArray) / sizeof(DEdge);
    // Helps us iterate through the answer
    //graph_traits<graph_t>::vertex_iterator i, iend;

    //graph_t kGraph(kEdgeArray, kEdgeArray + num_arcs, kWeights, kNumNodes);

    //property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, kGraph);
    //graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);

    //graph_t g(edge_array, edge_array + num_arcs, kWeights, kNumNodes);
    //property_map<graph_t, edge_weight_t>::type weight_map = get(edge_weight, kGraph);

    //vector<vertex_descriptor> p(num_vertices(kGraph));
    //vector<int> d(num_vertices(kGraph));
    //vertex_descriptor v_d = vertex(kEdgeArray[0], kGraph);

    //dijkstra_shortest_paths(kGraph, v_d, predecessor_map(&p[0]).distance_map(&d[0]));

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