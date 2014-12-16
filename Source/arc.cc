#include "../Headers/arc.h"

using namespace Flow;

Arc::Arc() {

}

Arc::Arc(VNode* v) {
    this->kNode = v;
}
Arc::~Arc() {

}

VNode* Arc::get_node() {
    return this->kNode;
}