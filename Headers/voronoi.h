#ifndef _VORONOI_H_
#define _VORONOI_H_

#pragma once

#include <boost/polygon/voronoi.hpp>
#include <boost/polygon/voronoi_diagram.hpp>
#include "map.h"

using namespace Flow;
using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;
using boost::polygon::x;
using boost::polygon::voronoi_edge;
using boost::polygon::y;
using boost::polygon::low;
using boost::polygon::high;

namespace Algorithms {

    // Required to interface with boost
    struct VoronoiPoint {
        int a;
        int b;
        VoronoiPoint(int x, int y) : a(x), b(y) {}
    };

    // Required to interface with boost
    struct VoronoiSegment {
        VoronoiPoint p0;
        VoronoiPoint p1;
        VoronoiSegment(int x1, int y1, int x2, int y2) : p0(x1, y1), p1(x2, y2) {}
    };

    class Voronoi {
    public:
        // Default constructor
        Voronoi();
        // Constructor given a map (deprecated)
        Voronoi(Map*);
        // Default destructor
        ~Voronoi();
        // Templated entry point for all algorithmic starts
        void start();

        // Retrieve the edges of the voronoi graph
        vector<VEdge*> get_edges();
        // Retrieve the vertices of the voronoi graph
        vector<VNode*> get_vertices();

    private:
        // Boost points
        vector<VoronoiPoint> kPoints;
        // List of all edges
        vector<VEdge*> kVoronoiEdges;
        // List of all vertices
        vector<VNode*> kVoronoiVertices;
        // Cells generated by boost voronoi
        vector<boost::polygon::voronoi_diagram<double>::cell_type> kCells;

        // Boost voronoi datastructure
        voronoi_diagram<double> kVoronoiDiagram;

        // Map holder
        Map* kMap;
        // Dimensions of the map
        int kMaxHeight, kMaxWidth, kMinHeight, kMinWidth;

        // Convert the Boost output to vedges
        void generate_edges();
        // Verify that our clipped edges aren't greater than our map size
        VEdge* create_edge(double, double, double, double);
        // Check to see if we've seen this vertice before
        bool in_vertices(VNode*);
        // Check to see if we've seen this vertice before
        bool in_edges(VEdge*);
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