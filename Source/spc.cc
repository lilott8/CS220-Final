#include "../Headers/spc.h"
#include "../Headers/btree.h"
#include <vector>
#include <boost/polygon/voronoi_diagram.hpp>
#include <claim.h>

using namespace Flow;

SPC::SPC(){}

SPC::~SPC(){}


void SPC::start() {
    claim("S/start: starting SPC", kDebug);

    vector<rTreePoint> results;

    for(VNode* node : kPins) {
        claim("Node: " + node->vnode_to_string(), kDebug);

        // TODO: get the querying to work!
        //kRtree.query(bgi::nearest(rTreePoint(node->get_x(), node->get_y()), 5), back_inserter(results));

        for(int x = 0;x<(int)results.size();x++) {
            claim("guess :" + to_string(x) + " is: ", kDebug);
        }

        claim("=========================", kDebug);
    }
}