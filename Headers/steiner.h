#ifndef _FORTUNE_H_
#define _FORTUNE_H_

#pragma once

#include "map.h"
#include "voronoi.h"
#include <math.h>

using namespace Utilities;
using namespace std;
using namespace Flow;

/**
* Steiner class that takes in nodes and generates candidate steiner points from those points.
*/

namespace FlowAlgorithms {

    // Data structure to contain a triangle
    struct SteinerTriangle {
        VNode* p1;
        VNode* p2;
        VNode* p3;

        int geometric_mean;

        SteinerTriangle(VNode* n1, VNode* n2, VNode* n3) {
            p1 = n1; p2 = n2; p3 = n3; }

        string triangle_to_string() {
            return p1->vnode_to_string() + "\t->\t" + p2->vnode_to_string() + "\t->\t" + p3->vnode_to_string();
        }
    };

    class Steiner {
    public:
        const double PI=3.14159265358979323846;

        Steiner();
        Steiner(Map*);
        Steiner(Map*, int);
        ~Steiner();
        // Templated entry point for all algorithmic starts
        void start();

        //void set_vertices(set<VNode*>);
        // Return steiner variables
        set<VNode*> get_steiner_points();
        set<VEdge*> get_steiner_edges();
        set<MapRoute*> get_routes();

    private:
        vector<SteinerTriangle*> kTriangles;    // list of triangles
        set<VNode*> kSteinerVertices;           // list of all candidate nodes generated
        set<VEdge*> kSteinerEdges;              // list of all edges that include Steiner points
        set<MapRoute*> kRoutes;                 // list of routes to be routed
        Map* kMap;                              // Map object
        set<VNode*> kPins;                      // work around for getting iterators to work
        int kSteinerCalculator;                 // determines how to calculate steiner points

        // convert nodes to triangles, exponential time
        void generate_steiner_triangle_exponential(VNode* [], int, int, int, int);
        void generate_steiner_triangle_linear();
        // Naive approach to generating steiner points, runs in linear time
        void generate_steiner_midpoint_linear();
        // Naive approach to generating steiner points, runs in exponential time
        void generate_steiner_midpoint_exponential();
        // Build the Steiner points from the triangle objects generated in previous methods
        void build_steiner_points();

        // Triangle approach to Steiner point generation
        void generate_steiner_point_from_triangle();
        // check for size of angle being less than 120 degrees
        bool verify_triangle_angles(SteinerTriangle*);

        // calculate the geometric mean of a triangle
        VNode* manhattan_geometric_mean(SteinerTriangle*);

        // euclidean geometric mean
        double euclidean_geometric_mean();
        // euclidean distance calculation
        double calculate_euclidean_distance(VNode*, VNode*);
        // Find the appropriate angle
        double find_bigger_angle(double, double, double);
        double find_other_angle(double, double, double);
        // Find a midpoint between 2 points.
        VEdge* calculate_midpoint(Point, Point);
    };
}
#endif