
#include "../Headers/vevent.h"
#include "../Headers/vnode.h"

using namespace Flow;

VEvent::VEvent(VNode* n) {
this->kNode = n;
    this->kPlaceEvent = true;
}

VEvent::~VEvent() {

}

VNode* VEvent::get_node() {
    return this->kNode;
}

bool VEvent::get_place_event() {
    return this->kPlaceEvent;
}