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

    private:
        VNode* kStart;
        VNode* kEnd;
        VNode* kLeft;
        VNode* kRight;

        VEdge* kNeighbor;


    };
}

#endif