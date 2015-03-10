#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#pragma once

#include "problem_object.h"
#include "steiner.h"
#include "spc.h"
#include "prim.h"

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
        static int calculate_manhattan_distance(VNode*, VNode*);

    private:
        Map *kMap;

        set<VEdge*> kEdges;      // generated edges
        set<VNode*> kVertices;      // list of all nodes, steiner and points

        //FlowAlgorithms::Prim *kPrim;
        FlowAlgorithms::Steiner *kSteiner;
        FlowAlgorithms::Voronoi *kVoronoi;
        FlowAlgorithms::SPC *kSPC;

        Optimization kOpt;

        //bool kIsEuclidean;          // Euclidean or Rectilinear space

        void project_vertices_on_map(set<VNode*>);
        void project_edges_on_map(set<VEdge*>);
    };
}
#endif