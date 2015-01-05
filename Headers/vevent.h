#ifndef _EVENT_H_
#define _EVENT_H_

#pragma once

#include "vnode.h"
#include "arc.h"

namespace Flow {
    class VEvent {
    public:
        VEvent(VNode*);
        ~VEvent();

        VNode* get_node();
        bool get_place_event();
    private:
        VNode* kNode;
        /**
        * TODO: figure out why we can't have circular pointers...
        */
        //Arc* kArc;

        // There are two types of events, a placed event, eg. a net or a place,
        // and a generated event from an intersection of parabolas.
        // If the kPlaceEvent is true, then it denotes a net or place,
        // If the kPlaceEvent is false, then it denotes an edge generation.
        bool kPlaceEvent;
    };
}
#endif