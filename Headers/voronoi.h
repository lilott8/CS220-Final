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

/**
* Container for the Voronoi boost library.  This takes a set of pins and generates a voronoi diagram from them
*/

namespace FlowAlgorithms {

    // Required to interface with boost
    struct VoronoiVertex {
        int a;
        int b;
        VoronoiVertex(int x, int y) : a(x), b(y) {}
    };

    // Required to interface with boost
    struct VoronoiSegment {
        VoronoiVertex p0;
        VoronoiVertex p1;
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
        set<VEdge*> get_edges();
        // Retrieve the vertices of the voronoi graph
        set<VNode*> get_vertices();
        // Retrieve the routes of the graph
        set<MapRoute*> get_routes();

    private:
        // Boost points
        vector<VoronoiVertex> kVertices;
        // List of all edges
        set<VEdge*> kVoronoiEdges;
        // List of all vertices
        set<VNode*> kVoronoiVertices;
        // List of s/t routes that must be computed
        set<MapRoute*> kRoutes;
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
        MapRoute* create_source_target_pair(double, double);
    };
}

/**
* Boost required initilization code.
* This code links my data structures with
* the boost library that handles voronoi construction
*/
namespace boost {
    namespace polygon {

        using namespace FlowAlgorithms;

        template <>
        struct geometry_concept<VoronoiVertex> {
            typedef point_concept type;
        };

        template <>
        struct point_traits<VoronoiVertex> {
            typedef int coordinate_type;

            static inline coordinate_type get(
                    const VoronoiVertex& point, orientation_2d orient) {
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
            typedef VoronoiVertex point_type;

            static inline point_type get(const VoronoiSegment& segment, direction_1d dir) {
                return dir.to_int() ? segment.p1 : segment.p0;
            }
        };
    }  // polygon
}  // boost



#endif