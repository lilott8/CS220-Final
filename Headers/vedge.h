#ifndef _VEDGE_H_
#define _VEDGE_H_

#pragma once

#include "vnode.h"
#include "edge.h"

using namespace Utilities;

/**
* Represents an edge on a map
*/

namespace Flow {
    class VEdge : public Edge {
    public:
        VEdge();
        VEdge(VNode*, VNode*, VNode*);
        VEdge(VNode*, VNode*);
        ~VEdge();

        static int kLastId;         // keeps track of the last id generated
        VNode* kStart;              // start of the edge
        VNode* kEnd;                // end of the edge
        VNode* kDirection;          // which direction is this edge going (deprecated)
        int kId;                    // Unique ID needed for boost libraries
        int kCost;                  // what is the cost of using this edge

        // convert data structure to string
        string vedge_to_string();

        // comparator for sets
        bool operator==(VEdge&);

        // return the next id, while incrementing it
        static int get_next_vedge_id();
    };
}

#endif