#include "../Headers/claim.h"
#include "../Headers/spm.h"
#include "../Headers/node.h"

using namespace Flow;
using namespace Utilities;

SPM::SPM() {

}

SPM::~SPM() {

}

void SPM::start() {
    Algorithm::start();
    claim("S/start: Starting SPM!", kDebug);
}