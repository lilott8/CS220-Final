#include "../Headers/claim.h"
#include "../Headers/fortune.h"
#include "../Headers/vevent.h"
#include "../Headers/arc.h"

using namespace Flow;

Fortune::Fortune() {
    kRoot = 0;
    kWidth = 10;
    kHeight = 10;
}

Fortune::Fortune(int h, int w) {
    kRoot = 0;
    kWidth = w;
    kHeight = h;
}

Fortune::~Fortune() {

}

void Fortune::start(priority_queue<VNode*, vector<VNode*>, CloserToOrigin> queue) {
    FlowAlgorithm::start(queue);
    claim("F/start: Starting Fortunes!", kDebug);

    VEvent* event;
    while(!queue.empty()) {
        // Create an event with the nodes that are placed
        event = new VEvent(queue.top());
        queue.pop();
        claim("F/s: Found site:" + event->get_node()->coords_to_string(), kDebug);
        kLastX = event->get_node()->get_x();
        if(this->kFalseEvent.find(event) != kFalseEvent.end()) {
            delete(event);
            this->kFalseEvent.erase(event);
            continue;
        }

        if(event->get_place_event()) {
            generate_parabola(event);
        } else {
            delete_parabola(event);
        }
        delete(event);
    }

    finish_beachline(kRoot);

}

void Fortune::generate_parabola(VEvent* e) {
    // If we don't have a root, create a new one.
    if(!kRoot){
        kRoot = new BinaryTree(e);
        claim("F/generate_parabola: Generating a new root node", kNote);
        return;
    }

    if(!kRoot->is_leaf() && kRoot->get_parabola_focus()->get_node()->get_y() - e->get_node()->get_y() < 1) {
        // Create a ew event for our root
        VEvent* fp = kRoot->get_parabola_focus();
        // Set some stuff for our root
        kRoot->set_leaf_status(true);
        kRoot->set_left_leaf( new BinaryTree(fp));
        kRoot->set_right_leaf(new BinaryTree(e));

        // Create a new node that houses our intersection
        VNode* s = new VNode((e->get_node()->get_x() + fp->get_node()->get_x())/2, kHeight);
        kPoints.push_back(s);

        if(e->get_node()->get_x() > fp->get_node()->get_x()) {
            kRoot->set_kedge(new VEdge(s, fp->get_node(), e->get_node()));
        }
        else {
            kRoot->set_kedge(new VEdge(s, e->get_node(), fp->get_node()));
        }

        kEdges.push_back(kRoot->get_kedge());
        // If we are looking a child, that hasn't changed, we can exit and ignore anything else.
        return;
    }

}

void Fortune::delete_parabola(VEvent * e) {
}

void Fortune::finish_beachline(BinaryTree * r) {
}