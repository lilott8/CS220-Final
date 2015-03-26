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


void Hadlock::route(VNode* s, VNode* t) {
    kSource = s;
    kTarget = t;

    route_recursive();
}

void Hadlock::route_recursive() {
    //claim("size of queue: " + to_string(kWaveFrontSourcePQ.size()), kDebug);
    // Base case 1: Not finding a solution
    if (kWaveFrontPQ.size() < 1) {
        claim("We could not successfully route: "
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
        //successful_routing = true;
        //claim("We found the sink!", kDebug);
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
    if (kTraceBack.size() > 0
            && is_adjacent(curr, kTraceBack.back())
            && curr->get_cost() <= kTraceBack.back()->get_cost()
            ) {
        kTraceBack.push_back(curr);
    }

    return;
}

bool Hadlock::is_adjacent(VNode* a, VNode* b) {
    int delta_x, delta_y;
    double result = Controller::calculate_manhattan_distance(a, b);

    // Calculate the deltas between x and y,
    // this will prevent the diagonal jumping
    delta_x = abs(a->get_x() - b->get_x());
    delta_y = abs(a->get_y() - b->get_y());
    return result <= 1 && (delta_x == 1 || delta_y == 1);
}

bool Hadlock::is_placeable(VNode* v) { return is_placeable(v->get_x(), v->get_y());}

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
    if(kMap->get_map().at(x).at(y)->get_type() != VNode::BLOCKED) {
        return true;
    }
    return true;
}

bool Hadlock::is_in_queue(VNode* a) {
    for(auto it = kWaveFront.begin(); it < kWaveFront.end(); ++it) {
        if(*it == a) {
            return true;
        }
    }
    return false;
}

vector<VNode*> Hadlock::get_adjacent_nodes(VNode* c) {
    vector<VNode*> results;
    VNode* temp = c;

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

int Hadlock::calculate_metric(VNode* curr, VNode* prev) {
    double previous_distance = Controller::calculate_euclidean_distance(prev, kTarget);

    if(Controller::calculate_euclidean_distance(curr, kTarget) <= previous_distance) {
        return prev->get_cost();
    } else {
        return prev->get_cost() + 1;
    }
}
