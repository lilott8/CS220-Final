#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#pragma once

#include "problem_object.h"
#include "steiner.h"
#include "spc.h"
#include "kruskal.h"
#include "prim.h"

using namespace Utilities;
using namespace Algorithms;

namespace Flow {

    class Controller {
    public:
        enum AlgoType {KRUSKAL, FORTUNE, SPM};      // algorithm enum
        enum Optimization {H_OPT, U_OPT, DEFAULT};  // kruskal's optimization enum

        Controller();
        Controller(ProblemObject*);
        Controller(ProblemObject*, AlgoType, Optimization);
        ~Controller();

        void start();
        void print_map();

        static int calculate_distance(int, int);

    private:
        Map *kMap;

        vector<VEdge*> kEdges;      // generated edges
        vector<VNode*> kVertices;      // list of all nodes, steiner and points

        Prim *kPrim;
        Steiner *kSteiner;
        Voronoi *kVoronoi;
        SPC *kSPC;

        Optimization kOpt;

        //bool kIsEuclidean;          // Euclidean or Rectilinear space

        void project_vertices_on_map(vector<VNode*>);
        void project_edges_on_map(vector<VEdge*>);
    };
}
#endif