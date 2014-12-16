#include "../Headers/claim.h"
#include "../Headers/fortune.h"
#include "../Headers/vevent.h"
#include "../Headers/arc.h"

using namespace Flow;

Fortune::Fortune() {

}

Fortune::~Fortune() {

}

void Fortune::start(priority_queue<VNode*, vector<VNode*>, CloserToOrigin> queue) {
    FlowAlgorithm::start(queue);
    claim("F/start: Starting Fortunes!", kDebug);

    VEvent* event;
    while(!queue.empty()) {
        event = new VEvent(queue.top());
        claim("F/s: Found site:" + event->get_node()->coords_to_string(), kDebug);
        queue.pop();
    }
}