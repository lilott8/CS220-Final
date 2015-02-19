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
        ~VEdge();

        VNode* kStart;
        VNode* kEnd;
        VNode* kDirection;

        string vedge_to_string();

        bool operator==(VEdge&);
    };
}

#endif