//#include <boost/polygon/voronoi.hpp>
#include "../Headers/controller.h"
#include "../Headers/claim.h"
#include "../Headers/vedge.h"
#include "../Headers/voronoi.h"
#include "../Headers/vnode.h"

using namespace FlowAlgorithms;
using namespace Flow;
using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;
using boost::polygon::x;
using boost::polygon::y;
using boost::polygon::low;
using boost::polygon::high;
using boost::polygon::voronoi_edge;

/**
* Default constructor, not used
*/
Voronoi::Voronoi(){}

/**
* Initialize the voronoi class
* @param map* Pointer to the map object that the controller has
*/
Voronoi::Voronoi(Map* m) {
    kMap = m;
    kMaxWidth = kMap->get_x()-1;
    kMaxHeight = kMap->get_y()-1;
    kMinHeight = 0;
    kMinWidth = 0;
}
/*
Voronoi::Voronoi(int max_x, int max_y, int min_x, int min_y ) {
    kMaxWidth = max_x-1;
    kMaxHeight = max_y-1;
    kMinHeight = min_y;
    kMinWidth = min_x;
}*/

/**
* Destructor
*/
Voronoi::~Voronoi(){}


/**
* Public interface to begin using the voronoi class
* This method will convert all the data to the necessary data internal datastructures,
* build and execute the boost voronoi library, and then convert the output back to
* our generic data structures
*/
void Voronoi::start() {
    vector<VoronoiSegment> segments = vector<VoronoiSegment>();

    //for(int x = 0; x < this->kMap->get_pins().size(); x++) {
    for(VNode* v : this->kMap->get_pins()) {
        kVertices.push_back(VoronoiVertex(v->get_x(), v->get_y()));
    }

    construct_voronoi(kVertices.begin(), kVertices.end(), segments.begin(), segments.end(), &kVoronoiDiagram);
    this->generate_edges();
}

/**
* Iterates the boost library cells and generates all the edges and vertices that
* were discovered in the graph creation.
*/
// Traversing Voronoi edges using cell iterator.  This will iterate edges twice
void Voronoi::generate_edges() {
    claim("F/generate_voronoi: Generating egdes now", kDebug);
    int id = 0;
    for (voronoi_diagram<double>::const_cell_iterator it = kVoronoiDiagram.cells().begin();
         it != kVoronoiDiagram.cells().end(); ++it) {
        const voronoi_diagram<double>::cell_type& cell = *it;
        const voronoi_diagram<double>::edge_type* edge = cell.incident_edge();
        kCells.push_back(cell);
        VEdge* vedge;
        // This is convenient way to iterate edges around Voronoi cell.
        do {
            if (edge->is_primary()) {
                if(edge->is_finite()) {
                    // With this check, we are insuring that each edge is only drawn once,
                    // because the edge could be a half edge, and this roots out half edges
                    if(edge->cell()->source_index() < edge->twin()->cell()->source_index()) {
                        vedge = create_edge(edge->vertex0()->x(), edge->vertex0()->y(),
                                edge->vertex1()->x(), edge->vertex1()->y());

                        //claim("F/generate_edges: inserting a record of " + to_string(id), kDebug);
                        kVoronoiEdges.insert(vedge);
                        //claim("F/generate_edgs: Edge: " + vedge->kStart->coords_to_string() + " to " + vedge->kEnd->coords_to_string(), kDebug);
                    }
                } else {
                    const voronoi_diagram<double>::vertex_type* v0 = edge->vertex0();

                    if(v0 != NULL) {
                        // Direction of infinite edge is perpendicular to direction
                        // between the points owning the two half edges.
                        // Take the rotated right vector and multiply by a large
                        // enough number to reach your bounding box
                        VoronoiVertex p1 = kVertices.at(edge->cell()->source_index());
                        VoronoiVertex p2 = kVertices.at(edge->twin()->cell()->source_index());
                        double end_x = (p1.b - p2.b) * 640;
                        double end_y = (p1.a - p2.b) * -640;

                        vedge = create_edge(v0->x(), v0->y(), end_x, end_y);
                        //claim("F/generate_edges: inserting a record of " + to_string(id), kDebug);
                        kVoronoiEdges.insert(vedge);
                        //claim("F/generate_edgs: *Edge: " + vedge->kStart->coords_to_string() + " to " + vedge->kEnd->coords_to_string(), kDebug);
                    }
                }
            }
            edge = edge->next();
            id++;
        } while (edge != cell.incident_edge());
    }
}

/**
* Retrieve the edges generated from the boost voronoi creator
* @return vector of edges
*/
set<VEdge*> Voronoi::get_edges() {
    return this->kVoronoiEdges;
}

/**
* Retrieve the vertices generated from the boost voronoi creator
* @return vector of vertices
*/
set<VNode*> Voronoi::get_vertices() {
    return this->kVoronoiVertices;
}

/**
* Build a new edge from the input.  This method will do several things:
* It first verifies that the coordinates given as input are not outside
* of the size of our map it will clip as necessary, it also verifies we
* haven't seen the edge or vertex before, and finally create the edge and
* vertices as needed.
* @param1 x1
* @param2 y1
* @param3 x2
* @param4 y2
*/
VEdge* Voronoi::create_edge(double start_x, double start_y, double end_x, double end_y) {

    int x1, y1, x2, y2;

    /**
    * Min height/widths are assumed 0 in this case
    */
    if (start_x < 0) {x1 = kMinWidth;}
    else if (start_x > kMaxWidth) {x1 = kMaxWidth;}
    else {x1 = round(start_x);}

    if (start_y < 0) {y1 = kMinHeight;}
    else if (start_y > kMaxHeight) {y1 = kMaxHeight;}
    else {y1 = round(start_y);}

    if (end_x < 0) {x2 = kMinWidth;}
    else if (end_x > kMaxWidth) {x2 = kMaxWidth;}
    else {x2 = round(end_x);}

    if (end_y < 0) {y2 = kMinHeight;}
    else if (end_y > kMaxHeight) {y2 = kMaxHeight;}
    else {y2 = round(end_y);}

    VEdge *e = new VEdge();


    e->kStart = Map::get_map().at(x1).at(y1);
    e->kEnd = Map::get_map().at(x2).at(y2);

    if(e->kStart->get_type() == VNode::NONE) {
        e->kStart->set_type(VNode::Type::VORONOI);
    }

    if(e->kEnd->get_type() == VNode::NONE) {
        e->kEnd->set_type(VNode::Type::VORONOI);
    }

    e->kCost = Controller::calculate_manhattan_distance(e->kStart, e->kEnd);

    kVoronoiVertices.insert(e->kStart);
    kVoronoiVertices.insert(e->kEnd);
    //claim("creating a node of: " + e->vedge_to_string(), kDebug);
    //claim("====================================", kDebug);
    return e;
}

