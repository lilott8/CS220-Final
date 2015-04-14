//
// Created by Jason on 3/26/15.
//

#include "../Headers/hadlock.h"
#include "../Headers/controller.h"

using namespace Utilities;
using namespace FlowAlgorithms;
using namespace Flow;

Hadlock::Hadlock(){}

Hadlock::Hadlock(Map* m) {
    kMap = m;
}

Hadlock::~Hadlock() {}

/**
 * Route two arbitrary nodes
 */
void Hadlock::route(VNode* s, VNode* t) {
    kSource = s;
    kTarget = t;

    kWaveFrontPQ.push(s);

    route_recursive();

    build_edges();

    clear_queues();
}

/**
 * Route a set of nodes
 */
void Hadlock::start(set<MapRoute*> routes) {

    claim("H/start: Size of routes: " + to_string(routes.size()), kDebug);

    for(auto r : routes) {
        route(r->pSource, r->pTarget);
    }
}

/**
 * Recursive algorithm using hadlock's algorithm for finding the path between two nodes
 */
void Hadlock::route_recursive() {

    // Base case 1: Not finding a solution
    if (kWaveFrontPQ.size() < 1) {
        claim("H/route_recursive: We could not successfully route: "
                + kSource->coords_to_string() + "->"
                + kTarget->coords_to_string(), kWarning);
        return;
    }
    // Grab the first record
    VNode* curr = kWaveFrontPQ.top();
    // pop off the first record
    kWaveFrontPQ.pop();

    // Base case 2: We found the sink
    if (curr == kTarget) {
        // add the sink to the trace_back
        kTraceBack.push_back(curr);
        return;
    }

    // Case 3: We still have places on the map to visit
    /**
    * Check each possibility of the next wavefront
    */
    vector<VNode*> adjacent = get_adjacent_nodes(curr);

    for (int x = 0; x < adjacent.size(); x++) {
        kWaveFrontPQ.push(adjacent.at(x));
        // TODO: Figure out how to iterate PQ's
        kWaveFront.push_back(adjacent.at(x));
    }

    route_recursive();

    // Handle the trace_back generation for the algorithm
    if ((kTraceBack.size() > 0
            && is_adjacent(curr, kTraceBack.back())
            && curr->get_cost() <= kTraceBack.back()->get_cost())
            || curr == kSource) {
        kTraceBack.push_back(curr);
    }

    return;
}

/**
 * Get the edges generated from all the routes
 */
std::set<VEdge*> Hadlock::get_edges() {
    return kEdges;
}

/**
 * Determines if two nodes are adjacent in manhattan space (left, right, up, down only)
 */
bool Hadlock::is_adjacent(VNode* a, VNode* b) {
    int delta_x, delta_y;
    double result = Controller::calculate_manhattan_distance(a, b);

    // Calculate the deltas between x and y,
    // this will prevent the diagonal jumping
    delta_x = abs(a->get_x() - b->get_x());
    delta_y = abs(a->get_y() - b->get_y());
    return result <= 1 && (delta_x == 1 || delta_y == 1);
}

/*
 * Wrapper method for is_placeable
 */
bool Hadlock::is_placeable(VNode* v) { return is_placeable(v->get_x(), v->get_y());}

/*
 * Determines the placeability of the coordinates on a euclidean space
 */
bool Hadlock::is_placeable(int x, int y) {
    if (x > kMap->get_x() - 1 || x < 0) {
        return false;
    }
    if (y > kMap->get_y() - 1 || y < 0) {
        return false;
    }
    if(kSource->get_x() == x && kSource->get_y() == y) {
        return true;
    }
    if(kTarget->get_x() == x && kTarget->get_y() == y) {
        return true;
    }
    // It doesn't matter type the node is as long as it's not a blockage
    if(kMap->get_map().at(x).at(y)->get_type() == VNode::BLOCKED) {
        return false;
    }
    return true;
}

/**
 * Have we seen this node before
 */
bool Hadlock::is_in_queue(VNode* a) {
    for(auto it = kWaveFront.begin(); it < kWaveFront.end(); ++it) {
        if(*it == a) {
            return true;
        }
    }
    return false;
}

/**
 * Return the nodes that are up, down, left, and right to the current position
 */
vector<VNode*> Hadlock::get_adjacent_nodes(VNode* c) {
    vector<VNode*> results;

    // (x, y+1)
    if (is_placeable(c->get_x(), c->get_y() + 1)) {
        if (!is_in_queue(kMap->get_map().at(c->get_x()).at(c->get_y()+1))) {
            kMap->get_map().at(c->get_x()).at(c->get_y()+1)
                    ->set_cost(calculate_metric(
                            kMap->get_map().at(c->get_x()).at(c->get_y()+1), c));
            results.push_back(kMap->get_map().at(c->get_x()).at(c->get_y()+1));
        }
    }
    // (x, y-1)
    if (is_placeable(c->get_x(), c->get_y() - 1)) {
        if (!is_in_queue(kMap->get_map().at(c->get_x()).at(c->get_y()-1))) {
            kMap->get_map().at(c->get_x()).at(c->get_y()-1)
                    ->set_cost(calculate_metric
                            (kMap->get_map().at(c->get_x()).at(c->get_y()-1), c));
            results.push_back(kMap->get_map().at(c->get_x()).at(c->get_y()-1));
        }
    }
    // (x+1, y)
    if (is_placeable(c->get_x() + 1, c->get_y())) {
        if (!is_in_queue(kMap->get_map().at(c->get_x()+1).at(c->get_y()))) {
            kMap->get_map().at(c->get_x()+1).at(c->get_y())
                    ->set_cost(calculate_metric
                            (kMap->get_map().at(c->get_x()+1).at(c->get_y()), c));
            results.push_back(kMap->get_map().at(c->get_x()+1).at(c->get_y()));
        }
    }
    // (x-1, y)
    if (is_placeable(c->get_x() - 1, c->get_y())) {
        if (!is_in_queue(kMap->get_map().at(c->get_x()-1).at(c->get_y()))) {
            kMap->get_map().at(c->get_x()-1).at(c->get_y())
                    ->set_cost(calculate_metric
                            (kMap->get_map().at(c->get_x()-1).at(c->get_y()), c));
            results.push_back(kMap->get_map().at(c->get_x()-1).at(c->get_y()));
        }
    }
    return results;
}

/**
 * Calculate the distance in an appropriate manner, given the plane we have created
 */
int Hadlock::calculate_metric(VNode* curr, VNode* prev) {
    double previous_distance = Controller::calculate_euclidean_distance(prev, kTarget);

    if(Controller::calculate_euclidean_distance(curr, kTarget) <= previous_distance) {
        return prev->get_cost();
    } else {
        return prev->get_cost() + 1;
    }
}

/**
 * Create the edges between the two routed nodes
 */
void Hadlock::build_edges() {

    VNode* edge_start = kTraceBack.front();
    kTraceBack.pop_front();

    VNode* next_edge = kTraceBack.front();
    kTraceBack.pop_front();

    Direction dir = edge_start->get_x() - next_edge->get_x() == 0 ? Direction::HORIZONTAL : Direction::VERTICAL;
    Direction new_dir;

    //claim("Size of ktraceback after pops: " + to_string(kTraceBack.size()), kDebug);

    // We take the first value, so we don't need to inspect it again
    for(int x=0;x<kTraceBack.size();x++) {
        next_edge = kTraceBack.at(x);
        new_dir = edge_start->get_x() - next_edge->get_x() == 0 ? Direction::HORIZONTAL : Direction::VERTICAL;

        if(dir != new_dir) {
            //claim("We switched from: " + to_string(dir) + " to " + to_string(new_dir), kDebug);
            //claim(edge_start->vnode_to_string() + "->" + kTraceBack.at(x-1)->vnode_to_string(), kDebug);
            // Create the edge from the start to the n-1 element
            if (x - 1 > 0) {
                kEdges.insert(new VEdge(edge_start, kTraceBack.at(x - 1)));
                //claim("adding edge: " + edge_start->vnode_to_string() + "\t -> \t" + kTraceBack.at(x-1)->vnode_to_string(), kDebug);
                // Our new start is the n-1 element, because that is the
                // intersecting element at the direction change
                edge_start = kTraceBack.at(x - 1);
                // Change the direction
                dir = new_dir;
            } else {
                //claim("adding edge: " + edge_start->vnode_to_string() + "\t -> \t" + kTraceBack.at(x)->vnode_to_string(), kDebug);
                kEdges.insert(new VEdge(edge_start, kTraceBack.at(x)));
                edge_start = kTraceBack.at(x);
                dir = new_dir;
            }
        }

        // Last case for when we are done iterating all our edges.
        if(next_edge == kSource) {
            //claim("adding edge: " + edge_start->vnode_to_string() + "\t -> \t" + next_edge->vnode_to_string(), kDebug);
            kEdges.insert(new VEdge(edge_start, next_edge));
        }
    }
    //claim("=========================================", kDebug);
}

/**
 * Simple way to wipe the container data structures and start fresh
 */
void Hadlock::clear_queues() {
    kTraceBack.clear();
    kWaveFront.clear();
    // ...
    kWaveFrontPQ = std::priority_queue<VNode*, vector<VNode*>, CompareNodes>();
}