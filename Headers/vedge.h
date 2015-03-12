#ifndef _VEDGE_H_
#define _VEDGE_H_

#pragma once

#include "vnode.h"
#include "edge.h"

using namespace Utilities;

namespace Flow {
    class VEdge : public Edge {
    public:
        VEdge();
        VEdge(VNode*, VNode*, VNode*);
        VEdge(VNode*, VNode*);
        ~VEdge();

        static int kLastId;

        VNode* kStart;
        VNode* kEnd;
        VNode* kDirection;
        int kId;

        int kCost;

        string vedge_to_string();

        bool operator==(VEdge&);

        static int get_next_vedge_id();
    };
}

#endif