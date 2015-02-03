#ifndef _FORTUNE_H_
#define _FORTUNE_H_

#pragma once

#include <set>
#include <boost/polygon/voronoi.hpp>
#include "flow_algo.h"

using namespace Utilities;
using namespace std;
using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;
using boost::polygon::x;
using boost::polygon::y;
using boost::polygon::low;
using boost::polygon::high;

#ifndef NULL
#define NULL 0
#endif
#define DELETED -2

#define le 0
#define re 1

/************************************************
*
* Fortune:
* This class is what defines fortune's algorithm
* Fortune's algorithm, on a high level, runs:
*   -A sweep line looks for sites
*   -As new sites are found, new parabolas are
*       generated with the site as the focus
*       (This is the beachline)
*   -As the parabolas grow they create breakpoints,
*       there are always 2 breakpoints generated,
*       which comprise an edge
*   -As parabolas on the beachline begin to
*       disappear, this creates the vertices
*   -Circle events are a form of vertice
*       discovery
*   -Once the sweepline crosses the clip-line,
*       the algorithm is done
*
* This algorithm uses 2 main datastructures to
* keep track of the data moving through its
* perspective: a Binary Tree and priority queue.
*
* The BTree maintains the beachline.  Each leaf
* is a site that defines an arc on the beach line.
* The interal structure is 2 nodes that store 2 sites
* that define the actual breakline.
*
* The priority queue stores the events, both circle
* and sites.  It always keeps it ordered such that
* the next event is always closest to the sweepline.
*
* There are 6 main ideas here:
*
* Sweepline:
*   This line is what moves through the euclidean
*   space by either the x or y coordinate.
*   It is responsible for detecting site-events
*
* Beachline:
*   A piecewise set of parabolas that sits behind
*   the sweepline and is responsible for drawing
*   the actual voronoi diagram
*
* Site Event:
*   A site event is a point on the euclidean
*   space in which is required for calculating
*   distance between 2 site events (eg. The pins
*   on the breadboard).
*
* Circle Event:
*   This is a special event, in which 2 parabolas
*   encapsulate another parabola.  This is
*   important because it is what is responsible
*   for destroying "old" parabolas.  At the center,
*   this is a vertex for an intersection of 3 edges
*
* Voronoi Diagram Edge:
*   This is drawn where there is an intersection
*   of 2 parabolas, or rather the breakpoints.
*
* Clip line:
*   The clip line is so our algorithm knows where
*   the max-x/y ends.  This forces us to finish
*   the parabolic/circle calculations once our
*   sweepline crosses our clipped-line.
*
************************************************/







namespace Flow {

    struct BPoint {
        int a;
        int b;
        BPoint(int x, int y) : a(x), b(y) {}
    };

    struct BSegment {
        BPoint p0;
        BPoint p1;
        BSegment(int x1, int y1, int x2, int y2) : p0(x1, y1), p1(x2, y2) {}
    };



    struct Freenode {
        struct Freenode *nextfree;
    };

    struct FreeNodeArrayList {
        struct Freenode *memory;
        struct FreeNodeArrayList *next;

    };

    struct Freelist {
        struct Freenode *head;
        int nodesize;
    };

    struct VoronoiPoint {
        float x, y;
    };

// structure used both for sites and for vertices
    struct Site {
        struct VoronoiPoint coord;
        int sitenbr;
        int refcnt;
    };


    struct VoronoiEdge {
        float a, b, c;
        struct Site *ep[2];
        struct Site *reg[2];
        int edgenbr;

    };

    struct GraphEdge {
        GraphEdge(float a, float b, float c, float d) {
            x1 = a; x2 = c; y1 = b; y2 = d;
        }
        GraphEdge(){}

        float x1, y1, x2, y2;
        struct GraphEdge *next;
    };


    struct Halfedge {
        struct Halfedge *ELleft, *ELright;
        struct VoronoiEdge *ELedge;
        int ELrefcnt;
        char ELpm;
        struct Site *vertex;
        float ystar;
        struct Halfedge *PQnext;
    };

    class Fortune : public FlowAlgorithm {
    public:
        Fortune();
        ~Fortune();
        // Run the algorithm, this will actually generate the edges and
        // interface with the private methods.
        void start(vector<VNode*>);





        void use_boost_voronoi(vector<VNode*>);
        int iterate_primary_edges1(const voronoi_diagram<double>& vd);
        int iterate_primary_edges2(const voronoi_diagram<double>& vd);
        int iterate_primary_edges3(const voronoi_diagram<double>& vd);


        void reset_iterator();

        bool get_next(float &, float &, float &, float &);
        bool verify_debug_answers(int flag = 0);

    private:

        /**
        * Methods
        */
        // entrance for building our voronoi graph
        bool generate_voronoi(int = 0);
        // clean up outsanding pointers
        void clean_up();
        void clean_up_edges();
        // free up any other memory that can be
        void free_init(struct Freelist *fl, int size);
        void make_free(struct Freenode *curr, struct Freelist *fl);
        // build the geometry for the voronoi
        void init_geom();
        void init_plot();

        void ref(struct Site *v);
        void deref(struct Site *v);
        // Vertex discovery
        void end_point(struct VoronoiEdge *e, int lr, struct Site *s);
        void make_vertex(struct Site *v);
        void out_triple(struct Site *s1, struct Site *s2, struct Site *s3);
        // Binary Tree maintenance
        void open_pl();
        void delete_el(struct Halfedge *he);
        void insert_el(struct Halfedge *lb, struct Halfedge *newHe);
        // Priority queue maintenance
        void insert_pq(struct Halfedge *he, struct Site *v, float offset);
        void delete_pq(struct Halfedge *he);

        void out_site(struct Site *s);
        void out_bisector(struct VoronoiEdge *e);
        void out_ep(struct VoronoiEdge *e);
        void out_vertex(struct Site *v);
        // Clips the line to the bounding box
        void clip_line(struct VoronoiEdge *e);
        void push_graph_edge(float x1, float y1, float x2, float y2);
        void line(float x1, float y1, float x2, float y2);
        // checks for and creates circle events
        void circle(float x, float y, float radius);
        void range(float minX, float minY, float maxX, float maxY);
        // strictly for debugging, verifies answer
        void init_answer_key();

        char * get_free(struct Freelist *fl);
        char * my_alloc(unsigned n);

        // the logic that controls the voronoi graph creation
        bool voronoi(int triangulate);

        // binary tree maintenance
        bool initialize_el();
        // priority queue maintenance
        bool initialize_pq();

        // priority queue stuff
        struct Halfedge * find_pq();
        struct VoronoiPoint min_pq();
        struct Halfedge * extract_min_pq();

        // Binary tree maintenance
        struct Halfedge * create_he(struct VoronoiEdge *e, int pm);
        struct Halfedge * create_he(), *ELleft(), *ELright(), *ELleftbnd();
        struct Halfedge * get_hash_el(int b);
        struct Halfedge * left_bound_el(struct VoronoiPoint *p);
        struct Halfedge * right_el(struct Halfedge *he);
        struct Site * left_reg(struct Halfedge *he);
        struct Site * right_reg(struct Halfedge *he);
        struct Halfedge * left_el(struct Halfedge *he);
        // Calculating beach line events
        struct VoronoiEdge * bisect(struct Site *s1, struct Site *s2);
        struct Site * intersect(struct Halfedge *el1, struct Halfedge *el2, struct VoronoiPoint *p = 0);
        // get the next site to process
        struct Site * next_one();

        // priority queue stuff
        int is_pq_empty();
        int bucket_pq(struct Halfedge *he);

        int right_of(struct Halfedge *el, struct VoronoiPoint *p);

        float dist(struct Site *s, struct Site *t);

        /**
        * Variables
        */
        struct Freelist kHFL;
        struct Freelist kSFL;
        struct Freelist kEFL;
        struct Site *kSites;
        struct Site *kBottomSite;
        struct Halfedge *kELLeftEnd, *kELRightEnd;
        struct Halfedge *kPQHash;
        struct Halfedge ** ELhash;

        int kNEdges;
        int kNVertices;
        int kELHashSize;
        int kTriangulate, kSorted, kPlot, kIsDebug;
        int kNSites;
        int kSiteIdx;
        int kSqrtNSites;
        int kPQHashSize;
        int kPQCount;
        int kPQMin;
        int kNtry, kTotalSearch;
        int kTotalAlloc;

        float kPXMin, kPXMax, kPYMin, kPYMax, kCRadius;
        float kXMin, kXMax, kYMin, kYMax, kDeltaX, kDeltaY;
        float kBorderMinX, kBorderMaxX, kBorderMinY, kBorderMaxY;
        float kMinDistanceBetweenSites;
        float const kEpsilon = .01;

        FreeNodeArrayList *kAllMemoryList;
        FreeNodeArrayList *kCurrentMemoryBlock;

        GraphEdge *kAllEdges;
        GraphEdge *kIteratorEdges;

        vector<GraphEdge*> kAnswerKey;
    };
}
int scomp(const void *p1, const void *p2);

namespace boost {
    namespace polygon {

        using namespace Flow;

        template <>
        struct geometry_concept<BPoint> {
            typedef point_concept type;
        };

        template <>
        struct point_traits<BPoint> {
            typedef int coordinate_type;

            static inline coordinate_type get(
                    const BPoint& point, orientation_2d orient) {
                return (orient == HORIZONTAL) ? point.a : point.b;
            }
        };

        template <>
        struct geometry_concept<BSegment> {
            typedef segment_concept type;
        };

        template <>
        struct segment_traits<BSegment> {
            typedef int coordinate_type;
            typedef BPoint point_type;

            static inline point_type get(const BSegment& segment, direction_1d dir) {
                return dir.to_int() ? segment.p1 : segment.p0;
            }
        };
    }  // polygon
}  // boost

#endif