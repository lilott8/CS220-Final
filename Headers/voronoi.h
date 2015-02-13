#ifndef _VORONOI_H_
#define _VORONOI_H_

#pragma once

#include <boost/polygon/voronoi.hpp>
#include <boost/polygon/voronoi_diagram.hpp>

using namespace Flow;
using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;
using boost::polygon::x;
using boost::polygon::voronoi_edge;
using boost::polygon::y;
using boost::polygon::low;
using boost::polygon::high;

namespace Algorithms {

    struct VoronoiPoint {
        int a;
        int b;
        VoronoiPoint(int x, int y) : a(x), b(y) {}
    };

    struct VoronoiSegment {
        VoronoiPoint p0;
        VoronoiPoint p1;
        VoronoiSegment(int x1, int y1, int x2, int y2) : p0(x1, y1), p1(x2, y2) {}
    };

    class Voronoi {
    public:
        Voronoi();
        Voronoi(int max_x, int max_y, int min_x = 0, int min_y = 0);
        ~Voronoi();

        void generate_voronoi(vector<VNode*>);

        vector<VEdge*> get_edges();

    private:
        void generate_edges();
        VEdge* create_edge(double, double, double, double);

        vector<VoronoiPoint> kPoints;
        vector<VEdge*> kEdges;
        vector<boost::polygon::voronoi_diagram<double>::cell_type> kCells;

        voronoi_diagram<double> kVoronoiDiagram;

        // Dimensions of the map
        int kMaxHeight, kMaxWidth, kMinHeight, kMinWidth;

    };
}

/**
* Boost required initilization code.
* This code links my data structures with
* the boost library that handles voronoi construction
*/
namespace boost {
    namespace polygon {

        using namespace Algorithms;

        template <>
        struct geometry_concept<VoronoiPoint> {
            typedef point_concept type;
        };

        template <>
        struct point_traits<VoronoiPoint> {
            typedef int coordinate_type;

            static inline coordinate_type get(
                    const VoronoiPoint& point, orientation_2d orient) {
                return (orient == HORIZONTAL) ? point.a : point.b;
            }
        };

        template <>
        struct geometry_concept<VoronoiSegment> {
            typedef segment_concept type;
        };

        template <>
        struct segment_traits<VoronoiSegment> {
            typedef int coordinate_type;
            typedef VoronoiPoint point_type;

            static inline point_type get(const VoronoiSegment& segment, direction_1d dir) {
                return dir.to_int() ? segment.p1 : segment.p0;
            }
        };
    }  // polygon
}  // boost



#endif