#ifndef _FORTUNE_H_
#define _FORTUNE_H_

#pragma once

#include "flow_algo.h"
#include "binary_tree.h"
#include <set>
#include "vparabola.h"

using namespace Utilities;
using namespace std;

namespace Flow {
    class Fortune : public FlowAlgorithm {
    public:
        Fortune();
        Fortune(int, int);
        ~Fortune();
        // Run the algorithm, this will actually generate the edges and
        // interface with the private methods.
        void start(priority_queue<VNode*, vector<VNode*>, CloserToOrigin>);
    private:
        set<VEvent*> kDeleted;      // false events, used for reference because of inability to delete from PQ
        set<VEvent*> kFalseEvent;   // False events

        int kLastY;                 // Last position of our y

        priority_queue<VEvent*, std::vector<VEvent *>, CompareEvent> queue; // PQ with events to process

        list<VEdge*> get_edges(list<VNode*>);               // helper function to start the VD generation

        VParabola* get_parabola_by_x(double xx);            // get the parabola that is nearest the "y" in the beachline

        VNode* get_edge_intersection(VEdge* a, VEdge* b);   // find the intersection between two edges

        double get_x_of_edge(VParabola* par, double y);     // return the x position of an interestion of parabolas
        double get_y(VNode * p, double x);                  // get the y coordinate of the given input

        void insert_parabola(VNode* p);                     // process the place event
        void remove_parabola(VEvent* e);                    // process the circle event
        void finish_edge(VParabola* n);                     // process any unfinished edges in the tree
        void check_circle(VParabola* b);                    // checks for a disappearing parabola

    };
}

#endif