#ifndef _EVENT_H_
#define _EVENT_H_

#pragma once

#include "vnode.h"
#include "arc.h"
#include "vparabola.h"

namespace Flow {
    class VEvent {
    public:
        VNode* point;       // the point at which an event occurs (top circle point for cirlces, focus point for place)
        bool pe;            // whether it is a place event
        double y;           // y coordinate of the "point"
        VParabola* arch;    // if it is a PE, then there is an arch above the event

        /**
        *   Constructor for the class
        *   p: point, at which the event occurs
        *   pev: whether it is a place event or not
        */
        VEvent(VNode* p, bool pev) {
            point	= p;
            pe		= pev;
            y		= p->get_y();
            arch	= 0;
        }
    };
}
#endif