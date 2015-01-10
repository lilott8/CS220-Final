#ifndef _COMPARATORS_H_
#define _COMPARATORS_H_

#pragma once

#include "vnode.h"
#include <cmath>
#include "claim.h"
#include "vevent.h"

using namespace Flow;

class CompareNodesFortune {
public:
    bool operator()(VNode &a, VNode &b) {
        return a.get_id() < b.get_id();
    }
};

class CloserToOrigin {
public:
    // Calculate the euclidean distance for our comparisons
    bool operator()(VNode *a, VNode *b) {
        //double a_dist = sqrt(((0 - a->get_x()) * (0 - a->get_x())) + ((0 - a->get_y()) * (0 - a->get_y())));
        //double b_dist = sqrt(((0 - b->get_x()) * (0 - b->get_x())) + ((0 - b->get_y()) * (0 - b->get_y())));
        //return a_dist >= b_dist;
        return a->get_x() > b->get_x();
    }
    bool operator()(Flow::VEvent *a, Flow::VEvent *b) {

    }
};

/**
* TODO: figure out a good compare method for this
*/
class CompareBeachLine {
public:
    bool operator()(VNode *a, VNode *b) {
        return true;
    }
};

class CompareEvent {
    //: public std::binary_function<VEvent*, VEvent*, bool>
public:
    bool operator()(const Flow::VEvent* l, const Flow::VEvent* r) const {
        return (l->y < r->y);
    }
};

#endif