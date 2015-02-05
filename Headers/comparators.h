#ifndef _COMPARATORS_H_
#define _COMPARATORS_H_

#pragma once

#include <cmath>
#include "vnode.h"
#include "claim.h"

namespace Flow {
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
            return a->get_y() < b->get_y();
        }
    };
}

#endif