#include "../Headers/claim.h"
#include "../Headers/map.h"
#include "../Headers/steiner.h"
#include <vector>
#include <boost/polygon/voronoi.hpp>
#include <voronoi.h>
//#include <boost/polygon/voronoi_visual_utils.hpp>

/**
* http://www.boost.org/doc/libs/1_57_0/libs/geometry/doc/html/geometry/spatial_indexes/rtree_quickstart.html
* http://www.boost.org/doc/libs/1_57_0/libs/geometry/doc/html/geometry/spatial_indexes/queries.html
* http://gallery.rcpp.org/articles/Rtree-examples/
* http://stackoverflow.com/questions/14195111/r-trees-should-i-reinvent-the-wheel
* http://stackoverflow.com/questions/5844939/how-to-construct-a-rtree-using-given-data-points
*
*/

using namespace Flow;
using namespace Algorithms;

Steiner::Steiner() {
}

Steiner::Steiner(Map* map) {
    this->kMap = map;
    kVoronoi = new Voronoi(kMap->get_x(), kMap->get_y());
}

Steiner::~Steiner() {
delete kVoronoi;
}


void Steiner::start() {
    kVoronoi->generate_voronoi(kMap->get_pins());
}




void Steiner::run_queries() {
    /*
    vector<rTreeValue> results;

    for (VNode *node : this->kMap->get_pins()) {
        claim("Node: " + node->vnode_to_string(), kDebug);

        //kRtree.query(bgi::nearest(rTreePoint(node->get_x(), node->get_y()), 5), back_inserter(results));

        if (results.size() > 0) {
            for (int x = 0; x < (int) results.size(); x++) {
                claim("guess :" + to_string(x) + " is: ", kDebug);
            }
        } else {
            claim("There were no nearest neighbors for: " + node->vnode_to_string(), kDebug);
        }
        results.clear();
        claim("=========================", kDebug);
    }*/
}