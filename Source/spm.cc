#include "../Headers/flow_algo.h"
#include "../Headers/claim.h"
#include "../Headers/spm.h"
#include "../Headers/node.h"

using namespace Flow;
using namespace Utilities;

SPM::SPM() {

}

SPM::~SPM() {

}

void SPM::start(priority_queue<VNode*, vector<VNode*>, CloserToOrigin> pq) {
    FlowAlgorithm::start(pq);
    claim("S/start: Starting SPM!", kDebug);
}