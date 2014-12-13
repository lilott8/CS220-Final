#ifndef _COMPARATORS_H_
#define _COMPARATORS_H_

#pragma once

#include "vnode.h"

using namespace Flow;

class CompareNodesFortune {
public:
    bool operator()(VNode &a, VNode &b) {
        return a.get_id() < b.get_id();
    }
};

class CloserToOrigin {
public:
    bool operator()(VNode *a, VNode *b) {
        return a->get_x() >= b->get_x() && a->get_y() >= b->get_y();
    }
};

#endif