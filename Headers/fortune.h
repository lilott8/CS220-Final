#ifndef _FORTUNE_H_
#define _FORTUNE_H_

#pragma once

#include <set>
#include "flow_algo.h"

using namespace Utilities;
using namespace std;

#ifndef NULL
#define NULL 0
#endif
#define DELETED -2

#define le 0
#define re 1

namespace Flow {
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
        void start(priority_queue<VNode*, vector<VNode*>, CloserToOrigin>);

        void reset_iterator();

        bool get_next(float &, float &, float &, float &);

    private:

        /**
        * Methods
        */
        bool generate_voronoi(int = 0);
        void clean_up();
        void clean_up_edges();
        void free_init(struct Freelist *fl, int size);
        void make_free(struct Freenode *curr, struct Freelist *fl);
        void init_geom();
        void init_plot();
        bool voronoi(int triangulate);
        void ref(struct Site *v);
        void deref(struct Site *v);
        void end_point(struct VoronoiEdge *e, int lr, struct Site *s);
        void delete_el(struct Halfedge *he);
        void make_vertex(struct Site *v);
        void out_triple(struct Site *s1, struct Site *s2, struct Site *s3);
        void insert_pq(struct Halfedge *he, struct Site *v, float offset);
        void delete_pq(struct Halfedge *he);
        bool initialize_el();
        void insert_el(struct Halfedge *lb, struct Halfedge *newHe);
        void out_site(struct Site *s);
        void out_bisector(struct VoronoiEdge *e);
        void out_ep(struct VoronoiEdge *e);
        void out_vertex(struct Site *v);
        void clip_line(struct VoronoiEdge *e);
        void push_graph_edge(float x1, float y1, float x2, float y2);
        void open_pl();
        void line(float x1, float y1, float x2, float y2);
        void circle(float x, float y, float radius);
        void range(float minX, float minY, float maxX, float maxY);

        char * get_free(struct Freelist *fl);
        char * my_alloc(unsigned n);

        struct VoronoiPoint min_pq();

        struct Halfedge ** ELhash;
        struct Halfedge * create_he(), *ELleft(), *ELright(), *ELleftbnd();
        struct Halfedge * create_he(struct VoronoiEdge *e, int pm);
        struct Halfedge * find_pq();
        struct Halfedge * extract_min_pq();
        struct Halfedge * left_bound_el(struct VoronoiPoint *p);
        struct Halfedge * right_el(struct Halfedge *he);
        struct Halfedge * get_hash_el(int b);
        struct Halfedge * left_el(struct Halfedge *he);

        struct VoronoiEdge * bisect(struct Site *s1, struct Site *s2);
        struct Site * intersect(struct Halfedge *el1, struct Halfedge *el2, struct VoronoiPoint *p = 0);
        struct Site * next_one();
        struct Site * right_reg(struct Halfedge *he);
        struct Site * left_reg(struct Halfedge *he);

        int is_pq_empty();
        int bucket_pq(struct Halfedge *he);
        int right_of(struct Halfedge *el, struct VoronoiPoint *p);

        float dist(struct Site *s, struct Site *t);

        /**
        * Variables
        */
        bool initialize_pq();

        struct Freelist kHFL;
        struct Freelist kSFL;
        struct Freelist kEFL;
        struct Site *kSites;
        struct Site *kBottomSite;
        struct Halfedge *kELLeftEnd, *kELRightEnd;
        struct Halfedge *kPQHash;

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

        FreeNodeArrayList *kAllMemoryList;
        FreeNodeArrayList *kCurrentMemoryBlock;

        GraphEdge *kAllEdges;
        GraphEdge *kIteratorEdges;
    };
}
int scomp(const void *p1, const void *p2);

#endif