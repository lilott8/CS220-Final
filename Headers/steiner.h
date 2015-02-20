#ifndef _FORTUNE_H_
#define _FORTUNE_H_

#pragma once

#include "map.h"
#include "voronoi.h"
#include "btree.h"
#include <math.h>

using namespace Utilities;
using namespace std;
using namespace Flow;

namespace Algorithms {

# define M_PI 3.14159265358979323846  /* pi */

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
        Steiner();
        ~Steiner();
        // Run the algorithm, this will actually generate the edges and
        void start();

        void set_vertices(vector<VNode*>);

    private:
        vector<VNode*> kAllVertices;
        vector<SteinerTriangle*> kTriangles;
        vector<VNode*> kSteinerPoints;

        void generate_triangles(VNode* [], int, int, int, int);
        void generate_steiner_points();

        double find_angle_size(VNode*, VNode*);
        VNode* manhattan_geometric_mean(SteinerTriangle*);
        double euclidean_geometric_mean();
    };
}
#endif