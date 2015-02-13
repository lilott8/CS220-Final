#include "../Headers/claim.h"
#include "fortune.h"
#include <vector>
#include <boost/polygon/voronoi.hpp>
//#include <boost/polygon/voronoi_visual_utils.hpp>

using namespace Flow;
using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;
using boost::polygon::x;
using boost::polygon::y;
using boost::polygon::low;
using boost::polygon::high;
using boost::polygon::voronoi_edge;


Fortune::Fortune() {
}

Fortune::~Fortune() {

}


void Fortune::start(vector<VNode*> queue) {
    FlowAlgorithm::start(queue);
    claim("F/start: starting the algorithm", kDebug);
    use_boost_voronoi(queue);
}

void Fortune::use_boost_voronoi(vector<VNode*> k) {
    vector<BSegment> segments = vector<BSegment>();

    for(int x = 0; x < k.size(); x++) {
        kPoints.push_back(BPoint(k.at(x)->get_x(), k.at(x)->get_y()));
    }

    voronoi_diagram<double> vd;
    construct_voronoi(kPoints.begin(), kPoints.end(), segments.begin(), segments.end(), &vd);

    // Iterate all the cells to obtain all the edges!
    claim("F/use_boost_vorono: Generating egdes now", kDebug);
    generate_edges(vd);
}

// Traversing Voronoi edges using cell iterator.
void Fortune::generate_edges(const voronoi_diagram<double> &vd) {
    int id = 0;
    for (voronoi_diagram<double>::const_cell_iterator it = vd.cells().begin();
         it != vd.cells().end(); ++it) {
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

                        kRtree.insert(make_pair(rTreePoint(vedge->kStart->get_x(), vedge->kStart->get_y()), id));
                        //claim("F/generate_edges: inserting a record of " + to_string(id), kDebug);
                        // make sure we increment the id!
                        id += 1;
                        kRtree.insert(make_pair(rTreePoint(vedge->kEnd->get_x(), vedge->kEnd->get_y()), id));
                        //claim("F/generate_edges: inserting a record of " + to_string(id), kDebug);
                        kEdges.push_back(vedge);
                        //claim("F/generate_edgs: Edge: " + vedge->kStart->coords_to_string() + " to " + vedge->kEnd->coords_to_string(), kDebug);
                    }
                } else {
                    const voronoi_diagram<double>::vertex_type* v0 = edge->vertex0();

                    if(v0 != NULL) {
                        // Direction of infinite edge is perpendicular to direction
                        // between the points owning the two half edges.
                        // Take the rotated right vector and multiply by a large
                        // enough number to reach your bounding box
                        BPoint p1 = kPoints.at(edge->cell()->source_index());
                        BPoint p2 = kPoints.at(edge->twin()->cell()->source_index());
                        double end_x = (p1.b - p2.b);// * 640;
                        double end_y = (p1.a - p2.b);// * -640;

                        vedge = create_edge(v0->x(), v0->y(), end_x, end_y);
                        kRtree.insert(make_pair(rTreePoint(vedge->kStart->get_x(), vedge->kStart->get_y()), id));
                        //claim("F/generate_edges: inserting a record of " + to_string(id), kDebug);
                        id += 1;
                        kRtree.insert(make_pair(rTreePoint(vedge->kEnd->get_x(), vedge->kEnd->get_y()), id));
                        //claim("F/generate_edges: inserting a record of " + to_string(id), kDebug);
                        kEdges.push_back(vedge);
                        //claim("F/generate_edgs: *Edge: " + vedge->kStart->coords_to_string() + " to " + vedge->kEnd->coords_to_string(), kDebug);
                    }
                }
            }
            edge = edge->next();
            id++;
        } while (edge != cell.incident_edge());
    }
}

VEdge* Fortune::create_edge(double start_x, double start_y, double end_x, double end_y) {

    int x1, y1, x2, y2;

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
    e->kStart = new VNode(x1, y1);
    e->kEnd = new VNode(x2, y2);
    e->kStart->set_type(VNode::Type::STEINER);
    e->kEnd->set_type(VNode::Type::STEINER);
    //claim("creating a node of: " + e->vedge_to_string(), kDebug);
    //claim("====================================", kDebug);
    return e;
}

void Fortune::run_queries() {
    vector<rTreeValue> results;

    for (VNode *node : kPins) {
        claim("Node: " + node->vnode_to_string(), kDebug);

        kRtree.query(bgi::nearest(rTreePoint(node->get_x(), node->get_y()), 5), back_inserter(results));

        if (results.size() > 0) {
            for (int x = 0; x < (int) results.size(); x++) {
                claim("guess :" + to_string(x) + " is: ", kDebug);
            }
        } else {
            claim("There were no nearest neighbors for: " + node->vnode_to_string(), kDebug);
        }
        results.clear();
        claim("=========================", kDebug);
    }
}