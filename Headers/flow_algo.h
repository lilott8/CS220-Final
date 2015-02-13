#ifndef _FLOW_ALGO_H_
#define _FLOW_ALGO_H_

#pragma once

#include <boost/polygon/voronoi.hpp>
#include <string>
#include <queue>
#include <list>
#include <queue>
#include <boost/geometry/index/rtree.hpp>

#include "vnode.h"
#include "vpath.h"
#include "comparators.h"
#include "vedge.h"
#include "btree.h"

using namespace Utilities;
using namespace std;
using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_cell;
namespace bgi = boost::geometry::index;
namespace bg = boost::geometry;

namespace Flow {

    typedef bg::model::point<int, 2, bg::cs::cartesian> rTreePoint;
    typedef bg::model::segment <rTreePoint> rTreeSegment;
    typedef bg::model::box <rTreePoint> rTreeBox;
    //typedef pair<rTreeSegment, size_t> rTreeValue;
    typedef pair<rTreePoint, size_t> rTreeValue;

    class FlowAlgorithm {
    public:
        enum AlgoType {KRUSKAL, FORTUNE, SPM};      // algorithm enum
        enum Optimization {H_OPT, U_OPT, DEFAULT};  // kruskal's optimization enum

        FlowAlgorithm();                            // constructor
        virtual ~FlowAlgorithm();                   // virtual destructor

        string print_algo_type(AlgoType);           // debugging for which algorithm is being used
        string print_optimization(Optimization);    // debugging to let us know what we kruskals opt is used

        void set_map_size(int, int, int=0, int=0);  // Alert our class of the sizes of the map
        void set_euclidean(bool);

        virtual void start(vector<VNode*>); // run the algo
        virtual void start();

        vector<boost::polygon::voronoi_diagram<double>::cell_type> get_cells();
        vector<VEdge*> get_edges();
    private:

    protected:
        vector<VNode*> kPins;       // Pins that are placed and need to be routed
        vector<VEdge*> kEdges;      // generated edges
        vector<boost::polygon::voronoi_diagram<double>::cell_type> kCells;

        int kMaxHeight;             // Max height of our map
        int kMaxWidth;              // Max width of our map
        int kMinWidth;              // Min width of our map
        int kMinHeight;             // Min height of map
        bool kIsEuclidean;          // Euclidean or Rectilinear space

        // Create the dynamic balancing rstar tree
        // the "16" dictates how many elements can be in an R-Tree node
        bgi::rtree<rTreeValue, bgi::rstar<16>> kRtree;

        void clear_all();           // clear the lists
    };
}

#endif