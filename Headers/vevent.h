#ifndef _VEVENT_H_
#define _VEVENT_H_

#pragma once

#include "vnode.h"
#include "vparabola.h"

namespace Flow{
    class VParabola;
}

namespace Flow {
    class VEvent {
    public:
        VEvent(VNode*, bool);   // constructor

        VNode* kPoint;       // the point at which an event occurs (top circle point for circles, focus point for place)
        bool kPlaceEvent;    // whether it is a place event
        double kYCoord;      // y coordinate of the "point"
        VParabola* kArch;    // if it is a PE, then there is an arch above the event
    };
}
#endif