#include <controller.h>
#include "../Headers/map.h"
#include "../Headers/steiner.h"

using namespace Flow;
using namespace FlowAlgorithms;

Steiner::Steiner() {
}

Steiner::Steiner(set<VNode*> v) {
    kAllVertices = v;
}

Steiner::~Steiner() {
}


void Steiner::start() {
    claim("S/start: Size of set of pins & voronoi points is: " + to_string(kAllVertices.size()), kDebug);
    //generate_steiner_intersections();
    generate_steiner_intersections_two();
}

set<VNode*> Steiner::get_steiner_points() {
    return kSteinerPoints;
}

set<VEdge*> Steiner::get_steiner_edges() {
    return kSteinerEdges;
}

void Steiner::generate_steiner_intersections() {
    for(VNode* outer : kAllVertices) {
        for(VNode* inner : kAllVertices) {
            if(outer == inner) {
                continue;
            }

            if(outer->get_x() == inner->get_x()) {
                // Calculate the midpoint
                int midpoint = (outer->get_y() + inner->get_y()) / 2;

                // Toggle the point on the map that needs to become a steiner point
                Map::get_map().at(outer->get_x()).at(midpoint)->set_type(VNode::Type::STEINER);
                // Add the steiner point to our array of candidate vertices
                kSteinerPoints.insert(Map::get_map().at(outer->get_x()).at(midpoint));

                // Save point x -> midpoint
                kSteinerEdges.insert(new VEdge(
                        outer, Map::get_map().at(outer->get_x()).at(midpoint)
                ));

                // save midpoint -> point y
                kSteinerEdges.insert(new VEdge(
                        Map::get_map().at(inner->get_x()).at(midpoint), inner
                ));
            } else if(outer->get_y() == inner->get_y()) {
                // Calculate the midpoint
                int midpoint = (outer->get_x() + inner->get_x()) / 2;

                // Toggle the point on the map that needs to become a steiner point
                Map::get_map().at(midpoint).at(outer->get_y())->set_type(VNode::Type::STEINER);
                // Add the steiner point to our array of candidate vertices
                kSteinerPoints.insert(Map::get_map().at(midpoint).at(outer->get_y()));

                // Save point x -> midpoint
                kSteinerEdges.insert(new VEdge(
                        outer, Map::get_map().at(midpoint).at(outer->get_y())
                ));
                // Save midpoint -> point y
                kSteinerEdges.insert(new VEdge(
                        Map::get_map().at(midpoint).at(inner->get_y()), inner
                ));
            } else {
                // Generate the x/y of the midpoint
                int mx, my;
                mx = (outer->get_x() + inner->get_x()) / 2;
                my = (outer->get_y() + inner->get_y()) / 2;

                // Toggle the point on the map that needs to become a steiner point
                Map::get_map().at(mx).at(my)->set_type(VNode::Type::STEINER);

                // Add the steiner point to our array of candidate vertices
                kSteinerPoints.insert(Map::get_map().at(mx).at(my));

                // Create the vedges for our new steiner points
                kSteinerEdges.insert(new VEdge(
                        outer, Map::get_map().at(mx).at(my)
                ));
                // Create the vedges for our new steiner points
                kSteinerEdges.insert(new VEdge(
                        Map::get_map().at(mx).at(my), inner
                ));
            }
        }
    }
}

void Steiner::generate_steiner_intersections_two() {
    int x = 0;
    VNode* inner;
    VNode* outer;
    
    std::set<VNode*>::iterator it;
    std::set<VNode*>::iterator it2;
    for(it = kAllVertices.begin(); it != kAllVertices.end(); ++it) {
        it2 = it;
        if(++it2 != kAllVertices.end()) {
            outer = *it;
            inner = *it2;
        }
        if(outer == inner) {
            continue;
        }
        
        if(outer->get_x() == inner->get_x()) {
            // Calculate the midpoint
            int midpoint = (outer->get_y() + inner->get_y()) / 2;

            // Toggle the point on the map that needs to become a steiner point
            Map::get_map().at(outer->get_x()).at(midpoint)->set_type(VNode::Type::STEINER);
            // Add the steiner point to our array of candidate vertices
            kSteinerPoints.insert(Map::get_map().at(outer->get_x()).at(midpoint));

            // Save point x -> midpoint
            kSteinerEdges.insert(new VEdge(
                    outer, Map::get_map().at(outer->get_x()).at(midpoint)
            ));

            // save midpoint -> point y
            kSteinerEdges.insert(new VEdge(
                    Map::get_map().at(inner->get_x()).at(midpoint), inner
            ));
        } else if(outer->get_y() == inner->get_y()) {
            // Calculate the midpoint
            int midpoint = (outer->get_x() + inner->get_x()) / 2;

            // Toggle the point on the map that needs to become a steiner point
            Map::get_map().at(midpoint).at(outer->get_y())->set_type(VNode::Type::STEINER);
            // Add the steiner point to our array of candidate vertices
            kSteinerPoints.insert(Map::get_map().at(midpoint).at(outer->get_y()));

            // Save point x -> midpoint
            kSteinerEdges.insert(new VEdge(
                    outer, Map::get_map().at(midpoint).at(outer->get_y())
            ));
            // Save midpoint -> point y
            kSteinerEdges.insert(new VEdge(
                    Map::get_map().at(midpoint).at(inner->get_y()), inner
            ));
        } else {
            // Generate the x/y of the midpoint
            int mx, my;
            mx = (outer->get_x() + inner->get_x()) / 2;
            my = (outer->get_y() + inner->get_y()) / 2;

            // Toggle the point on the map that needs to become a steiner point
            Map::get_map().at(mx).at(my)->set_type(VNode::Type::STEINER);

            // Add the steiner point to our array of candidate vertices
            kSteinerPoints.insert(Map::get_map().at(mx).at(my));

            // Create the vedges for our new steiner points
            kSteinerEdges.insert(new VEdge(
                    outer, Map::get_map().at(mx).at(my)
            ));
            // Create the vedges for our new steiner points
            kSteinerEdges.insert(new VEdge(
                    Map::get_map().at(mx).at(my), inner
            ));
        }
    }
}