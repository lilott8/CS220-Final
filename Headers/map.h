#ifndef _MAP_H_
#define _MAP_H_

#pragma once

#include "vnode.h"
#include "problem_object.h"
#include "vedge.h"
#include <queue>
#include "comparators.h"
#include <set>
// Swap routine for drawing lines
#define mapswap(a, b) do{ __typeof__(a) tmp;  tmp = a; a = b; b = tmp; }while(0)

using namespace Utilities;

/**
* Handles the map object which holds all the data for our problem.
*/

namespace Flow {
    //
    struct MapRoute {
        MapRoute(){};
        MapRoute(VNode* s, VNode* t) {pSource = s; pTarget = t;};
        VNode* pSource;
        VNode* pTarget;
    };

    class Map {
    public:
        Map();
        Map(ProblemObject*);
        ~Map();

        // print map to stdout
        void print_map();
        // alter the state of a node on the map
        void set(VNode* node);

        // get a list of pins that need to be routed
        std::set<VNode*> get_pins();
        std::set<VNode*> get_corners();
        vector<MapRoute*> get_routes();
        VNode* get_closest_node(VNode*);
        // return the map, able to be referenced everywhere
        static vector<vector<VNode*>> get_map();

        void draw_voronoi_edges(vector<VEdge*>);
        // get the dimensions of the map
        int get_x();
        int get_y();

    private:
        int kHeight;                // max dimension of the y-plane
        int kWidth;                 // max dimension of the x-plane
        int kDefaultSize = 10;      // default size if needed
        bool kIsEuclidean;          // is this a euclidean map or not
        std::set<VNode*> kPins;     // list of all the pins
        std::set<VNode*> kCorners;  // list of all the obstacle corners
        vector<MapRoute*> kRoutes;
        //static vector<vector<VNode*>> kMap;

        // begin building the datastructures to contain the map
        void initialize_map();
        // add the pins to the map
        void set_pins(vector<Connection>);
        // add the blockages to the map
        void set_blockages(vector<Blocker>);
        // line drawing algorithms
        void draw_bresenham_lines(vector<VEdge*>);

        void generate_hanan_grid();
    };
}

#endif