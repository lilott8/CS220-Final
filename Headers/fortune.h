#ifndef _FORTUNE_H_
#define _FORTUNE_H_

#pragma once

#include <set>
#include <boost/polygon/voronoi.hpp>
#include "flow_algo.h"

using namespace Utilities;
using namespace std;
using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;
using boost::polygon::x;
using boost::polygon::y;
using boost::polygon::low;
using boost::polygon::high;

namespace Flow {

    struct BPoint {
        int a;
        int b;
        BPoint(int x, int y) : a(x), b(y) {}
    };

    struct BSegment {
        BPoint p0;
        BPoint p1;
        BSegment(int x1, int y1, int x2, int y2) : p0(x1, y1), p1(x2, y2) {}
    };

    class Fortune : public FlowAlgorithm {
    public:
        Fortune();
        ~Fortune();
        // Run the algorithm, this will actually generate the edges and
        // interface with the private methods.
        void start(vector<VNode*>);
        void use_boost_voronoi(vector<VNode*>);
        void run_queries();
    private:
        /**
        * Methods
        */
        VEdge* create_edge(double, double, double, double);
        void generate_edges(const voronoi_diagram<double>& vd);

        /**
        * Variables
        */
        vector<BPoint> kPoints;    // list of pins converted to BPoints
    };
}

/**
* Boost required initilization code.
* This code links my data structures with
* the boost library that handles voronoi construction
*/
namespace boost {
    namespace polygon {

        using namespace Flow;

        template <>
        struct geometry_concept<BPoint> {
            typedef point_concept type;
        };

        template <>
        struct point_traits<BPoint> {
            typedef int coordinate_type;

            static inline coordinate_type get(
                    const BPoint& point, orientation_2d orient) {
                return (orient == HORIZONTAL) ? point.a : point.b;
            }
        };

        template <>
        struct geometry_concept<BSegment> {
            typedef segment_concept type;
        };

        template <>
        struct segment_traits<BSegment> {
            typedef int coordinate_type;
            typedef BPoint point_type;

            static inline point_type get(const BSegment& segment, direction_1d dir) {
                return dir.to_int() ? segment.p1 : segment.p0;
            }
        };
    }  // polygon
}  // boost

#endif