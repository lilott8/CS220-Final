#ifndef _ARC_H_
#define _ARC_H_

#pragma once

#include "../Headers/vnode.h"
#include "../Headers/pathsegment.h"
#include "../Headers/path.h"
#include "../Headers/vevent.h"
#include "../Headers/vedge.h"

using namespace Utilities;

namespace Flow {
    class Arc {
    public:
        Arc();
        Arc(VNode *);
        ~Arc();

        VNode* get_node();

    private:
        VNode* kNode;
        VEdge* kEdge;
        /**
        * TODO: figure out why we can't have circular pointers...
        */
        //VEvent* kEvent;
        Arc* kArc;
    };
}
#endif