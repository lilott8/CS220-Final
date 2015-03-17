#ifndef _MAP_H_
#define _MAP_H_

#pragma once

#include "vnode.h"
#include "problem_object.h"
#include "vedge.h"
#include <queue>
#include "comparators.h"
#include <set>

#define plot_(X,Y,D) do{ rgb_color f_;				\
  f_.red = r; f_.green = g; f_.blue = b;			\
  _dla_plot(img, (X), (Y), &f_, (D)) ; }while(0)

// Swap routine for drawing lines
#define mapswap(a, b) do{ __typeof__(a) tmp;  tmp = a; a = b; b = tmp; }while(0)

using namespace Utilities;

/**
* Handles the map object which holds all the data for our problem.
*/

namespace Flow {

    //
    struct Route {
        Route(){};
        VNode pStart;
        VNode pStop;
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
        //static vector<vector<VNode*>> kMap;

        // begin building the datastructures to contain the map
        void initialize_map();
        // add the pins to the map
        void set_pins(vector<Connection>);
        // add the blockages to the map
        void set_blockages(vector<Blocker>);
        // line drawing algorithms
        void draw_xiaolin_wu_lines(vector<VEdge*>);
        void draw_bresenham_lines(vector<VEdge*>);
    };
}

#endif