
#include "../Headers/vevent.h"
#include "../Headers/vnode.h"

using namespace Flow;

/**
*   Constructor for the class
*/
VEvent::VEvent(VNode* p, bool pev) {
    kPoint= p;
    kPlaceEvent= pev;
    kYCoord= p->get_y();
    kArch= 0;
}