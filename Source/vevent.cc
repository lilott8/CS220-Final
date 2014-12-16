
#include "../Headers/vevent.h"
#include "../Headers/vnode.h"

using namespace Flow;

VEvent::VEvent(VNode* n) {
this->kNode = n;
}

VEvent::~VEvent() {

}

VNode* VEvent::get_node() {
    return this->kNode;
}