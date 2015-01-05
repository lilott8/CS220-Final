#ifndef _FORTUNE_H_
#define _FORTUNE_H_

#pragma once

#include "flow_algo.h"
#include "binary_tree.h"
#include <set>

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
        list<VNode*> kNodes;        // Vertices
        list<VEdge*> kEdges;        // Edges
        BinaryTree* kRoot;          // Root of the BTree
        int kLastX;                 // Last position of our x
        set<VEvent*> kFalseEvent;   // False events
        list<VNode*> kPoints;       // Newly discovered points from the algorithm
        int kWidth;
        int kHeight;

        // deprecated in favor of wrapping the datastructure into the parent class
        //priority_queue<VNode*, vector<VNode*>, CloserToOrigin> kQueue; //events to process

        // process the place
        void generate_parabola(VEvent*);
        // process the circle
        void delete_parabola(VEvent *);
        // make sure we have visited everything on the map, if not, visit it!
        void finish_beachline(BinaryTree*);
        // return the x-coordinate at which two parabola's intersect
        double get_parabola_intersection(BinaryTree*, double);
        // get the parabola that is "below" this point
        BinaryTree get_parabola_under_x_coordinate(double);
        // Find when the circle event of Fortune's occur!
        void check_circle(BinaryTree*);

        void get_intersection(VEdge*, VEdge*);

    };
}

#endif