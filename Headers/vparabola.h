#ifndef VParabola_h
#define VParabola_h

#include "vnode.h"
#include "vedge.h"
#include "vevent.h"

/*
	Parabolas and events have pointers to each other, so we declare class VEvent, which will be defined later.
*/

class VEvent;

/*
	A class that stores information about an item in beachline sequence (see Fortune's algorithm).
	It can represent an arch of parabola or an intersection between two archs (which defines an edge).
	In my implementation I build a binary tree with them (internal nodes are edges, leaves are archs).
*/
namespace Flow {
    class VParabola {
    public:

        /*
        isLeaf		: flag whether the node is Leaf or internal node
        site		: pointer to the focus point of parabola (when it is parabola)
        edge		: pointer to the edge (when it is an edge)
        cEvent		: pointer to the event, when the arch disappears (circle event)
        parent		: pointer to the parent node in tree
    */

        bool kIsLeaf;
        VNode *kSite;
        VEdge *kEdge;
        VEvent *kCEvent;
        VParabola *kParent;

        /*
        Constructors of the class (empty for edge, with focus parameter for an arch).
    */

        VParabola();
        VParabola(VNode *s);

        /*
        Access to the children (in tree).
    */

        void set_left(VParabola* p) {
            kLeft = p;
            p->kParent = this;
        }

        void set_right(VParabola* p) {
            kRight = p;
            p->kParent = this;
        }

        VParabola* left() {
            return kLeft;
        }

        VParabola* right() {
            return kRight;
        }

        /*
        Some useful tree operations

        GetLeft			: returns the closest left leave of the tree
        GetRight		: returns the closest right leafe of the tree
        GetLeftParent	: returns the closest parent which is on the left
        GetLeftParent	: returns the closest parent which is on the right
        GetLeftChild	: returns the closest leave which is on the left of current node
        GetRightChild	: returns the closest leave which is on the right of current node
    */

        static VParabola* get_left(VParabola *p);
        static VParabola* get_right(VParabola *p);
        static VParabola* get_left_parent(VParabola *p);
        static VParabola* get_right_parent(VParabola *p);
        static VParabola* get_left_child(VParabola *p);
        static VParabola* get_right_child(VParabola *p);

    private:
        VParabola* kLeft;
        VParabola* kRight;
    };
}

#endif