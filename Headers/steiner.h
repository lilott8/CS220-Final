#ifndef _FORTUNE_H_
#define _FORTUNE_H_

#pragma once

#include <set>
#include "map.h"
#include "voronoi.h"

using namespace Utilities;
using namespace std;
using namespace Flow;

namespace Algorithms {

    class Steiner {
    public:
        Steiner();
        Steiner(Map*);
        ~Steiner();
        // Run the algorithm, this will actually generate the edges and
        void start();

    private:
        void run_queries();

        vector<VoronoiPoint> kPoints;    // list of pins converted to BPoints

        vector<VEdge*> kEdges;
        Map* kMap;
        Voronoi* kVoronoi;

    };
}
#endif