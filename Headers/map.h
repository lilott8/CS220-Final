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

#define mapswap(a, b) do{ __typeof__(a) tmp;  tmp = a; a = b; b = tmp; }while(0)

using namespace Utilities;

namespace Flow {

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

        void print_map();
        void draw_voronoi_edges(vector<VEdge*>);
        void set(VNode* node);

        std::set<VNode*> get_pins();
        static vector<vector<VNode*>> get_map();

        int get_x();
        int get_y();

    private:
        int kHeight;
        int kWidth;
        int kDefaultSize = 10;
        bool kIsEuclidean;
        std::set<VNode*> kPins;
        //static vector<vector<VNode*>> kMap;

        void initialize_map();
        void set_pins(vector<Connection>);
        void set_blockages(vector<Blocker>);
        void draw_xiaolin_wu_lines(vector<VEdge*>);
        void draw_bresenham_lines(vector<VEdge*>);
    };
}

#endif