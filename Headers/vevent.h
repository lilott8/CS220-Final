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
    private:
        VNode* kNode;
        /**
        * TODO: figure out why we can't have circular pointers...
        */
        //Arc* kArc;
        bool kPlaceEvent;
    };
}
#endif