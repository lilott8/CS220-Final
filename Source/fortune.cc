#include "../Headers/claim.h"
#include "../Headers/Fortune.h"

using namespace Flow;

Fortune::Fortune() {

}

Fortune::~Fortune() {

}

void Fortune::start() {
    Algorithm::start();
    claim("F/start: Starting Fortunes!", kDebug);
}