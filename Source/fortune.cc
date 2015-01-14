#include "../Headers/claim.h"
#include "../Headers/fortune.h"

using namespace Flow;

Fortune::Fortune() {
    kSiteIdx = 0;
    kSites = 0;

    kAllMemoryList = new FreeNodeArrayList;
    kAllMemoryList->memory = 0;
    kAllMemoryList->next = 0;
    kCurrentMemoryBlock = kAllMemoryList;
    kAllEdges = 0;
    kIteratorEdges = 0;
    kMinDistanceBetweenSites = 0;
}

Fortune::Fortune(double h, double w) {
    kWidth = w;
    kHeight = h;
}

Fortune::~Fortune() {
    clean_up();
    clean_up_edges();

    if(kAllMemoryList != 0)
        delete kAllMemoryList;
}

void Fortune::start(priority_queue<VNode*, vector<VNode*>, CloserToOrigin> queue) {
    FlowAlgorithm::start(queue);
    claim("F/start: Starting Fortunes!", kDebug);

    float xValues[10] = {0, 9, 0, 9, 4, 5, 5, 3, 2, 7};
    float yValues[10] = {0, 9, 9, 0, 4, 6, 2, 8, 3, 3};

    this->generate_voronoi(xValues, yValues, 10, 0, 10, 0, 10, 0);

    float x1,y1,x2,y2;

    this->reset_iterator();

    printf("\n-------------------------------\n");
    int x = 0;

    while(get_next(x1,y1,x2,y2)) {
        printf("GOT Line #%d (%f,%f)->(%f,%f)\n",x, x1,y1,x2, y2);
        x++;
    }
}

void Fortune::reset_iterator() {
    kIteratorEdges = kAllEdges;
}

bool Fortune::get_next(float &x1, float &y1, float &x2, float &y2) {
    if (kIteratorEdges == 0) {
        return false;
    }

    x1 = kIteratorEdges->x1;
    x2 = kIteratorEdges->x2;
    y1 = kIteratorEdges->y1;
    y2 = kIteratorEdges->y2;

    kIteratorEdges = kIteratorEdges->next;

    return true;
}

/**
*
*/
bool Fortune::generate_voronoi(float *xValues, float *yValues, int numPoints, float minX, float maxX, float minY, float maxY, float minDist) {
    clean_up();
    clean_up_edges();
    int i;

    kMinDistanceBetweenSites = minDist;

    kNSites=numPoints;
    kPlot = 0;
    kTriangulate = 0;
    kIsDebug = 1;
    kSorted = 0;
    free_init(&kSFL, sizeof (Site));

    kSites = (struct Site *) my_alloc(kNSites*sizeof( *kSites));

    if(kSites == 0) {
        return false;
    }

    kXMin = xValues[0];
    kYMin = yValues[0];
    kXMax = xValues[0];
    kYMax = yValues[0];

    for(i = 0; i< kNSites; i++) {
        kSites[i].coord.x = xValues[i];
        kSites[i].coord.y = yValues[i];
        kSites[i].sitenbr = i;
        kSites[i].refcnt = 0;

        if (xValues[i] < kXMin) {
            kXMin = xValues[i];
        } else if (xValues[i] > kXMax) {
            kXMax = xValues[i];
        }

        if (yValues[i] < kYMin) {
            kYMin = yValues[i];
        } else if (yValues[i] > kYMax) {
            kYMax = yValues[i];
        }
    }

    qsort(kSites, kNSites, sizeof (*kSites), scomp);

    kSiteIdx = 0;
    init_geom();
    float temp = 0;
    if(minX > maxX) {
        temp = minX;
        minX = maxX;
        maxX = temp;
    }

    if(minY > maxY) {
        temp = minY;
        minY = maxY;
        maxY = temp;
    }

    kBorderMinX = minX;
    kBorderMinY = minY;
    kBorderMaxX = maxX;
    kBorderMaxY = maxY;

    kSiteIdx = 0;
    voronoi(kTriangulate);

    return true;
}

/**
*
*/
bool Fortune::initialize_el() {
    int i;
    free_init(&kHFL, sizeof **ELhash);
    kELHashSize = 2 * kSqrtNSites;
    ELhash = (struct Halfedge **) my_alloc ( sizeof *ELhash * kELHashSize);

    if(ELhash == 0) {
        return false;
    }

    for(i=0; i<kELHashSize; i +=1) {
        ELhash[i] = (struct Halfedge *)NULL;
    }
    kELLeftEnd = create_he( (struct VoronoiEdge *)NULL, 0);
    kELRightEnd = create_he( (struct VoronoiEdge *)NULL, 0);
    kELLeftEnd->ELleft = (struct Halfedge *)NULL;
    kELLeftEnd->ELright = kELRightEnd;
    kELRightEnd->ELleft = kELLeftEnd;
    kELRightEnd->ELright = (struct Halfedge *)NULL;
    ELhash[0] = kELLeftEnd;
    ELhash[kELHashSize-1] = kELRightEnd;

    return true;
}

/**
*
*/
struct Halfedge* Fortune::create_he(struct VoronoiEdge *e, int pm) {
    struct Halfedge *answer;
    answer = (struct Halfedge *) get_free(&kHFL);
    answer->ELedge = e;
    answer->ELpm = pm;
    answer->PQnext = (struct Halfedge *) NULL;
    answer->vertex = (struct Site *) NULL;
    answer->ELrefcnt = 0;
    return(answer);
}

/**
*
*/
void Fortune::insert_el(struct Halfedge *lb, struct Halfedge *newHe) {
    newHe->ELleft = lb;
    newHe->ELright = lb->ELright;
    (lb->ELright)->ELleft = newHe;
    lb->ELright = newHe;
}

/**
*
*/
/* Get ekNtry from hash table, pruning any deleted nodes */
struct Halfedge* Fortune::get_hash_el(int b) {
    struct Halfedge *he;

    if(b<0 || b>=kELHashSize) {
        return ((struct Halfedge *) NULL);
    }

    he = ELhash[b];

    if (he == (struct Halfedge *) NULL || he->ELedge != (struct VoronoiEdge *) DELETED ) {
        return (he);
    }

    /* Hash table points to deleted half edge.  Patch as necessary. */
    ELhash[b] = (struct Halfedge *) NULL;
    if ((he->ELrefcnt -= 1) == 0) {
        make_free((Freenode *) he, &kHFL);
    }

    return ((struct Halfedge *) NULL);
}

/**
*
*/
struct Halfedge * Fortune::left_bound_el(struct VoronoiPoint *p) {
    int i, bucket;
    struct Halfedge *he;

    /* Use hash table to get close to desired halfedge */
    bucket = (int)((p->x - kXMin)/kDeltaX * kELHashSize); // use the hash function to find the place in the hash map that this HalfEdge should be

    if(bucket<0) bucket =0; // make sure that the bucket position in within the range of the hash array
    if(bucket>=kELHashSize) bucket = kELHashSize - 1;

    he = get_hash_el(bucket);

    // if the HE isn't found, search backwards and forwards in the hash map for the first non-null ekNtry
    if(he == (struct Halfedge *) NULL) {
        for(i=1; 1 ; i += 1) {
            if ((he=get_hash_el(bucket-i)) != (struct Halfedge *) NULL) {
                break;
            }
            if ((he=get_hash_el(bucket+i)) != (struct Halfedge *) NULL) {
                break;
            }
        }
        kTotalSearch += i;
    }
    // This one possibly
    kNtry++;

    /* Now search linear list of halfedges for the correct one */
    if (he==kELLeftEnd  || (he != kELRightEnd && right_of(he,p))) {
        do {
            he = he->ELright;
        } while (he!=kELRightEnd && right_of(he,p)); // keep going right on the list until either the end is reached, or you find the 1st edge which the point
        he = he->ELleft; // isn't to the right of
    } else { // if the point is to the left of the HalfEdge, then search left for the HE just to the left of the point
        do {
            he = he->ELleft;
        } while (he != kELLeftEnd && !right_of(he, p));
    }
    /* Update hash table and reference counts */
    if(bucket > 0 && bucket <kELHashSize-1) {
        if(ELhash[bucket] != (struct Halfedge *) NULL) {
            ELhash[bucket]->ELrefcnt--;
        }
        ELhash[bucket] = he;
        // This one possibly
        ELhash[bucket]->ELrefcnt++;
    }
    return (he);
}

/**
*
*/
/* This delete routine can't reclaim node, since pointers from hash
table may be present.   */
void Fortune::delete_el(struct Halfedge *he) {
    (he->ELleft)->ELright = he->ELright;
    (he->ELright)->ELleft = he->ELleft;
    he->ELedge = (struct VoronoiEdge *)DELETED;
}

/**
*
*/
struct Halfedge* Fortune::right_el(struct Halfedge *he) {
    return (he->ELright);
}
/**
*
*/
struct Halfedge* Fortune::left_el(struct Halfedge *he) {
    return (he->ELleft);
}

/**
*
*/
struct Site* Fortune::left_reg(struct Halfedge *he) {
    if(he->ELedge == (struct VoronoiEdge *)NULL) {
        return (kBottomSite);
    }
    return(he->ELpm == le ? he->ELedge->reg[le] : he->ELedge->reg[re]);
}

/**
*
*/
struct Site * Fortune::right_reg(struct Halfedge *he) {
    // if this halfedge has no edge, return the bottom site (whatever that is)
    if(he->ELedge == (struct VoronoiEdge *)NULL) {
        return (kBottomSite);
    }

    // if the ELpm field is zero, return the site 0 that this edge bisects, otherwise return site number 1
    return( he -> ELpm == le ? he->ELedge -> reg[re] : he->ELedge->reg[le]);
}

/**
*
*/
void Fortune::init_geom() {
    float sn;

    free_init(&kEFL, sizeof(VoronoiEdge));
    kNVertices = 0;
    kNEdges = 0;
    sn = (float)kNSites+4;
    kSqrtNSites = (int)sqrt(sn);
    kDeltaY = kYMax - kYMin;
    kDeltaX = kXMax - kXMin;
}

/**
*
*/
struct VoronoiEdge * Fortune::bisect(struct Site *s1, struct Site *s2) {
    float dx,dy,adx,ady;
    struct VoronoiEdge *newedge;

    newedge = (struct VoronoiEdge *) get_free(&kEFL);

    newedge->reg[0] = s1; // store the kSites that this edge is bisecting
    newedge->reg[1] = s2;
    ref(s1);
    ref(s2);
    newedge->ep[0] = (struct Site *) NULL; // to begin with, there are no endpoints on the bisector - it goes to infinity
    newedge->ep[1] = (struct Site *) NULL;

    dx = s2->coord.x - s1->coord.x; // get the difference in x dist between the kSites
    dy = s2->coord.y - s1->coord.y;
    adx = dx>0 ? dx : -dx; // make sure that the difference in positive
    ady = dy>0 ? dy : -dy;
    newedge->c = (float)(s1->coord.x * dx + s1->coord.y * dy + (dx*dx + dy*dy)*0.5); // get the slope of the line

    if (adx>ady) {
        newedge->a = 1.0;
        newedge->b = dy/dx;
        newedge->c /= dx; // set formula of line, with x fixed to 1
    } else {
        newedge->b = 1.0;
        newedge->a = dx/dy;
        newedge->c /= dy; // set formula of line, with y fixed to 1
    };

    newedge->edgenbr = kNEdges;

    // This one possibly
    kNEdges++;
    return(newedge);
}

/**
*
*/
//create a new site where the HalfEdges el1 and el2 intersect - note that the Point in the argument list is not used, don't know why it's there
struct Site * Fortune::intersect(struct Halfedge *el1, struct Halfedge *el2, struct VoronoiPoint *p) {
    struct VoronoiEdge *e1,*e2, *e;
    struct Halfedge *el;
    float d, xint, yint;
    int right_of_site;
    struct Site *v;

    e1 = el1->ELedge;
    e2 = el2->ELedge;
    if(e1 == (struct VoronoiEdge*)NULL || e2 == (struct VoronoiEdge*)NULL) {
        return ((struct Site *) NULL);
    }

    //if the two edges bisect the same parent, return null
    if (e1->reg[1] == e2->reg[1]) {
        return ((struct Site *) NULL);
    }

    d = e1->a * e2->b - e1->b * e2->a;
    if (-1.0e-10<d && d<1.0e-10) {
        return ((struct Site *) NULL);
    }

    xint = (e1->c*e2->b - e2->c*e1->b)/d;
    yint = (e2->c*e1->a - e1->c*e2->a)/d;

    if( (e1->reg[1]->coord.y < e2->reg[1]->coord.y) ||
            (e1->reg[1]->coord.y == e2->reg[1]->coord.y &&
                    e1->reg[1]->coord.x < e2->reg[1]->coord.x) ) {
        el = el1;
        e = e1;
    } else {
        el = el2;
        e = e2;
    }

    right_of_site = xint >= e->reg[1]->coord.x;
    if ((right_of_site && el->ELpm == le) || (!right_of_site && el->ELpm == re)) {
        return ((struct Site *) NULL);
    }

    //create a new site at the point of intersection - this is a new vector event waiting to happen
    v = (struct Site *) get_free(&kSFL);
    v->refcnt = 0;
    v->coord.x = xint;
    v->coord.y = yint;
    return(v);
}

/**
*
*/
/* returns 1 if p is to right of halfedge e */
int Fortune::right_of(struct Halfedge *el, struct VoronoiPoint *p) {
    struct VoronoiEdge *e;
    struct Site *topsite;
    int right_of_site, above, fast;
    float dxp, dyp, dxs, t1, t2, t3, yl;

    e = el->ELedge;
    topsite = e->reg[1];
    right_of_site = p->x > topsite->coord.x;

    if(right_of_site && el->ELpm == le) {
        return(1);
    }

    if(!right_of_site && el->ELpm == re) {
        return (0);
    }

    if (e->a == 1.0) {
        dyp = p->y - topsite->coord.y;
        dxp = p->x - topsite->coord.x;
        fast = 0;

        if ((!right_of_site & (e->b < 0.0)) | (right_of_site & (e->b >= 0.0))) {
            above = dyp >= e->b * dxp;
            fast = above;
        } else {
            above = p->x + p->y * e->b > e->c;

            if (e->b < 0.0) {
                above = !above;
            }

            if (!above) {
                fast = 1;
            }
        }

        if (!fast) {
            dxs = topsite->coord.x - (e->reg[0])->coord.x;
            above = e->b * (dxp * dxp - dyp * dyp) <
                    dxs * dyp * (1.0 + 2.0 * dxp / dxs + e->b * e->b);
            if (e->b < 0.0) {
                above = !above;
            }
        }
    } else {
    /*e->b==1.0 */
    	yl = e->c - e->a*p->x;
        t1 = p->y - yl;
        t2 = p->x - topsite->coord.x;
        t3 = yl - topsite->coord.y;
        above = t1*t1 > t2*t2 + t3*t3;
    }
    return (el->ELpm==le ? above : !above);
}

/**
*
*/
void Fortune::end_point(struct VoronoiEdge *e, int lr, struct Site * s) {
    e->ep[lr] = s;
    ref(s);
    if(e->ep[re-lr]== (struct Site *) NULL) {
        return;
    }

    clip_line(e);

    deref(e->reg[le]);
    deref(e->reg[re]);
    make_free((Freenode*)e, &kEFL);
}

/**
*
*/
float Fortune::dist(struct Site *s, struct Site *t) {
    float dx,dy;
    dx = s->coord.x - t->coord.x;
    dy = s->coord.y - t->coord.y;
    return (float)(sqrt(dx*dx + dy*dy));
}

/**
*
*/
void Fortune::make_vertex(struct Site *v) {
    v->sitenbr = kNVertices;
    // this one possibly
    kNVertices++;
    out_vertex(v);
}

/**
*
*/
void Fortune::deref(struct Site *v) {
    v->refcnt -= 1;

    if(v->refcnt == 0) {
        make_free((Freenode *) v, &kSFL);
    }
}

/**
*
*/
void Fortune::ref(struct Site *v) {
    v->refcnt += 1;
}

/**
*
*/
//push the HalfEdge into the ordered linked list of vertices
void Fortune::insert_pq(struct Halfedge *he, struct Site * v, float offset) {
    struct Halfedge *last, *next;

    he->vertex = v;
    ref(v);
    he->ystar = (float)(v -> coord.y + offset);
    last = &kPQHash[bucket_pq(he)];

    while((next = last->PQnext) != (struct Halfedge *) NULL &&
            (he->ystar > next->ystar  ||
                    (he->ystar == next->ystar && v->coord.x > next->vertex->coord.x))) {
        last = next;
    }
    he->PQnext = last->PQnext;
    last->PQnext = he;

    // this one possibly
    kPQCount++;
}

/**
*
*/
//remove the HalfEdge from the list of vertices
void Fortune::delete_pq(struct Halfedge *he) {
    struct Halfedge *last;

    if(he->vertex != (struct Site *) NULL) {
        last = &kPQHash[bucket_pq(he)];
        while(last->PQnext != he) {
            last = last->PQnext;
        }

        last->PQnext = he->PQnext;
        // this one possibly
        kPQCount--;
        deref(he->vertex);
        he->vertex = (struct Site *) NULL;
    }
}

/**
*
*/
int Fortune::bucket_pq(struct Halfedge *he) {
    int bucket;

    bucket = (int)((he->ystar - kYMin)/kDeltaY * kPQHashSize);

    if (bucket<0) {
        bucket = 0;
    }

    if (bucket>=kPQHashSize) {
        bucket = kPQHashSize-1;
    }

    if (bucket < kPQMin) {
        kPQMin = bucket;
    }
    return(bucket);
}


/**
*
*/
int Fortune::is_pq_empty() {
    return(kPQCount==0);
}

/**
*
*/
struct VoronoiPoint Fortune::min_pq() {
    struct VoronoiPoint answer;

    while(kPQHash[kPQMin].PQnext == (struct Halfedge *)NULL) {
        // this one possible
        kPQMin++;
    }
    answer.x = kPQHash[kPQMin].PQnext->vertex->coord.x;
    answer.y = kPQHash[kPQMin].PQnext->ystar;
    return (answer);
}

/**
*
*/
struct Halfedge * Fortune::extract_min_pq() {
    struct Halfedge *curr;

    curr = kPQHash[kPQMin].PQnext;
    kPQHash[kPQMin].PQnext = curr->PQnext;
    // this one possibly
    kPQCount--;
    return(curr);
}

/**
*
*/
bool Fortune::initialize_pq() {
    int i;

    kPQCount = 0;
    kPQMin = 0;
    kPQHashSize = 4 * kSqrtNSites;
    kPQHash = (struct Halfedge *) my_alloc(kPQHashSize * sizeof *kPQHash);

    if(kPQHash == 0) {
        return false;
    }

    for(i=0; i<kPQHashSize; i+=1) {
        kPQHash[i].PQnext = (struct Halfedge *)NULL;
    }

    return true;
}

/**
*
*/
void Fortune::free_init(struct Freelist *fl, int size) {
    fl->head = (struct Freenode *) NULL;
    fl->nodesize = size;
}

/**
*
*/
char * Fortune::get_free(struct Freelist *fl) {
    int i;
    struct Freenode *t;

    if(fl->head == (struct Freenode *) NULL) {
        t =  (struct Freenode *) my_alloc(kSqrtNSites * fl->nodesize);

        if(t == 0) {
            return 0;
        }

        kCurrentMemoryBlock->next = new FreeNodeArrayList;
        kCurrentMemoryBlock = kCurrentMemoryBlock->next;
        kCurrentMemoryBlock->memory = t;
        kCurrentMemoryBlock->next = 0;

        for(i=0; i<kSqrtNSites; i+=1) {
            make_free((struct Freenode *) ((char *) t + i * fl->nodesize), fl);
        }
    }

    t = fl->head;
    fl->head = (fl->head)->nextfree;
    return((char *)t);
}

/**
*
*/
void Fortune::make_free(struct Freenode *curr, struct Freelist *fl) {
    curr->nextfree = fl->head;
    fl->head = curr;
}

/**
*
*/
void Fortune::clean_up() {
    if(kSites != 0) {
        free(kSites);
        kSites = 0;
    }

    FreeNodeArrayList* current=0, *prev = 0;

    current = prev = kAllMemoryList;

    while(current->next != 0) {
        prev = current;
        current = current->next;
        free(prev->memory);
        delete prev;
        prev = 0;
    }

    if(current != 0 && current->memory != 0) {
        free(current->memory);
        delete current;
    }

    kAllMemoryList = new FreeNodeArrayList;
    kAllMemoryList->next = 0;
    kAllMemoryList->memory = 0;
    kCurrentMemoryBlock = kAllMemoryList;
}

/**
*
*/
void Fortune::clean_up_edges() {
    GraphEdge* geCurrent = 0, *gePrev = 0;
    geCurrent = gePrev = kAllEdges;

    while(geCurrent != 0 && geCurrent->next != 0) {
        gePrev = geCurrent;
        geCurrent = geCurrent->next;
        delete gePrev;
    }

    kAllEdges = 0;

}

/**
*
*/
void Fortune::push_graph_edge(float x1, float y1, float x2, float y2) {
    GraphEdge* newEdge = new GraphEdge;
    newEdge->next = kAllEdges;
    kAllEdges = newEdge;
    newEdge->x1 = x1;
    newEdge->y1 = y1;
    newEdge->x2 = x2;
    newEdge->y2 = y2;
}

/**
*
*/
char * Fortune::my_alloc(unsigned n) {
    char *t=0;
    t=(char*)malloc(n);
    kTotalAlloc += n;
    return(t);
}

/**
*
*/
/* for those who don't have Cherry's kPlot */
/* #include <kPlot.h> */
void Fortune::open_pl(){}
void Fortune::line(float x1, float y1, float x2, float y2) {
    push_graph_edge(x1,y1,x2,y2);

}
void Fortune::circle(float x, float y, float radius){}
void Fortune::range(float minX, float minY, float maxX, float maxY){}



void Fortune::out_bisector(struct VoronoiEdge *e) {


}


void Fortune::out_ep(struct VoronoiEdge *e) {


}

void Fortune::out_vertex(struct Site *v) {

}


void Fortune::out_site(struct Site *s) {
    if(!kTriangulate & kPlot & !kIsDebug) {
        circle(s->coord.x, s->coord.y, kCRadius);
    }
}


void Fortune::out_triple(struct Site *s1, struct Site *s2, struct Site * s3) {

}

/**
*
*/
void Fortune::init_plot() {
    float dx,dy,d;

    dy = kYMax - kYMin;
    dx = kXMax - kXMin;
    d = (float)(( dx > dy ? dx : dy) * 1.1);
    kPXMin = (float)(kXMin - (d-dx)/2.0);
    kPXMax = (float)(kXMax + (d-dx)/2.0);
    kPYMin = (float)(kYMin - (d-dy)/2.0);
    kPYMax = (float)(kYMax + (d-dy)/2.0);
    kCRadius = (float)((kPXMax - kPXMin)/350.0);
    open_pl();
    range(kPXMin, kPYMin, kPXMax, kPYMax);
}

/**
*
*/
void Fortune::clip_line(struct VoronoiEdge *e) {
    struct Site *s1, *s2;
    float x1=0,x2=0,y1=0,y2=0, temp = 0;;

    x1 = e->reg[0]->coord.x;
    x2 = e->reg[1]->coord.x;
    y1 = e->reg[0]->coord.y;
    y2 = e->reg[1]->coord.y;

    //if the distance between the two points this line was created from is less than
    //the square root of 2, then ignore it
    if(sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))) < kMinDistanceBetweenSites) {
        return;
    }

    kPXMin = kBorderMinX;
    kPXMax = kBorderMaxX;
    kPYMin = kBorderMinY;
    kPYMax = kBorderMaxY;

    if(e->a == 1.0 && e->b >= 0.0) {
        s1 = e->ep[1];
        s2 = e->ep[0];
    } else {
        s1 = e->ep[0];
        s2 = e->ep[1];
    }

    if(e->a == 1.0) {
        y1 = kPYMin;
        if (s1!=(struct Site *)NULL && s1->coord.y > kPYMin) {
            y1 = s1->coord.y;
        }

        if(y1>kPYMax) {
            //	printf("\nClipped (1) y1 = %f to %f",y1,kPYMax);
            y1 = kPYMax;
            //return;
        }
        x1 = e->c - e->b * y1;
        y2 = kPYMax;
        if (s2!=(struct Site *)NULL && s2->coord.y < kPYMax) {
            y2 = s2->coord.y;
        }

        if(y2<kPYMin) {
            //printf("\nClipped (2) y2 = %f to %f",y2,kPYMin);
            y2 = kPYMin;
            //return;
        }

        x2 = (e->c) - (e->b) * y2;
        if (((x1> kPXMax) & (x2>kPXMax)) | ((x1<kPXMin)&(x2<kPXMin))) {
            //printf("\nClipLine jumping out(3), x1 = %f, kPXMin = %f, kPXMax = %f",x1,kPXMin,kPXMax);
            return;
        }
        if(x1> kPXMax) {
            x1 = kPXMax; y1 = (e->c - x1)/e->b;
        }

        if(x1<kPXMin) {
            x1 = kPXMin; y1 = (e->c - x1)/e->b;
        }

        if(x2>kPXMax) {
            x2 = kPXMax; y2 = (e->c - x2)/e->b;
        }

        if(x2<kPXMin) {
            x2 = kPXMin; y2 = (e->c - x2)/e->b;
        }
    } else {
        x1 = kPXMin;
        if (s1!=(struct Site *)NULL && s1->coord.x > kPXMin) {
            x1 = s1->coord.x;
        }

        if(x1>kPXMax) {
            //printf("\nClipped (3) x1 = %f to %f",x1,kPXMin);
            //return;
            x1 = kPXMax;
        }

        y1 = e->c - e->a * x1;
        x2 = kPXMax;
        if (s2!=(struct Site *)NULL && s2->coord.x < kPXMax) {
            x2 = s2->coord.x;
        }

        if(x2<kPXMin) {
            //printf("\nClipped (4) x2 = %f to %f",x2,kPXMin);
            //return;
            x2 = kPXMin;
        }
        y2 = e->c - e->a * x2;
        if (((y1> kPYMax) & (y2>kPYMax)) | ((y1<kPYMin)&(y2<kPYMin))) {
            //printf("\nClipLine jumping out(6), y1 = %f, kPYMin = %f, kPYMax = %f",y2,kPYMin,kPYMax);
            return;
        }

        if(y1> kPYMax) {
            y1 = kPYMax; x1 = (e -> c - y1)/e -> a;
        }

        if(y1<kPYMin) {
            y1 = kPYMin; x1 = (e -> c - y1)/e -> a;
        }

        if(y2>kPYMax) {
            y2 = kPYMax; x2 = (e -> c - y2)/e -> a;
        }

        if(y2<kPYMin) {
            y2 = kPYMin; x2 = (e -> c - y2)/e -> a;
        }
    }

    //printf("\nPushing line (%f,%f,%f,%f)",x1,y1,x2,y2);
    line(x1,y1,x2,y2);
}


/* implicit parameters: kNSites, kSqrtNSites, kXMin, kXMax, kYMin, kYMax,
kDeltaX, kDeltaY (can all be estimates).
Performance suffers if they are wrong; better to make kNSites,
kDeltaX, and kDeltaY too big than too small.  (?) */

bool Fortune::voronoi(int triangulate) {
    struct Site *newsite, *bot, *top, *temp, *p;
    struct Site *v;
    struct VoronoiPoint newintstar;
    int pm;
    struct Halfedge *lbnd, *rbnd, *llbnd, *rrbnd, *bisector;
    struct VoronoiEdge *e;

    initialize_pq();
    kBottomSite = next_one();
    out_site(kBottomSite);
    bool retval = initialize_el();

    if(!retval) {
        return false;
    }

    newsite = next_one();

    while(1) {

        if(!is_pq_empty()) {
            newintstar = min_pq();
        }

        //if the lowest site has a smaller y value than the lowest vector intersection, process the site
        //otherwise process the vector intersection

        /* new site is smallest - this is a site event*/
        if (newsite != (struct Site *)NULL 	&& (is_pq_empty() || newsite->coord.y < newintstar.y
                || (newsite->coord.y == newintstar.y && newsite->coord.x < newintstar.x))) {
            out_site(newsite); // output the site
            lbnd = left_bound_el(&(newsite->coord)); // get the first HalfEdge to the LEFT of the new site
            rbnd = right_el(lbnd); // get the first HalfEdge to the RIGHT of the new site
            bot = right_reg(lbnd); // if this halfedge has no edge, , bot = bottom site (whatever that is)
            e = bisect(bot, newsite); // create a new edge that bisects
            bisector = create_he(e, le); // create a new HalfEdge, setting its ELpm field to 0
            insert_el(lbnd, bisector); // insert this new bisector edge between the left and right vectors in a linked list

            //if the new bisector intersects with the left edge, remove the left edge's vertex, and put in the new one
            if ((p = intersect(lbnd, bisector)) != (struct Site *) NULL) {
                delete_pq(lbnd);
                insert_pq(lbnd, p, dist(p,newsite));
            }

            lbnd = bisector;
            bisector = create_he(e, re); // create a new HalfEdge, setting its ELpm field to 1
            insert_el(lbnd, bisector); // insert the new HE to the right of the original bisector earlier in the IF stmt

            // if this new bisector intersects with the
            if ((p = intersect(bisector, rbnd)) != (struct Site *) NULL) {
                insert_pq(bisector, p, dist(p,newsite)); // push the HE into the ordered linked list of vertices
            }
            newsite = next_one();
        } else if (!is_pq_empty()) { /* intersection is smallest - this is a vector event */
            lbnd = extract_min_pq(); // pop the HalfEdge with the lowest vector off the ordered list of vectors
            llbnd = left_el(lbnd); // get the HalfEdge to the left of the above HE
            rbnd = right_el(lbnd); // get the HalfEdge to the right of the above HE
            rrbnd = right_el(rbnd); // get the HalfEdge to the right of the HE to the right of the lowest HE
            bot = left_reg(lbnd); // get the Site to the left of the left HE which it bisects
            top = right_reg(rbnd); // get the Site to the right of the right HE which it bisects

            out_triple(bot, top, right_reg(lbnd)); // output the triple of kSites, stating that a circle goes through them

            v = lbnd->vertex; // get the vertex that caused this event
            make_vertex(v); // set the vertex number - couldn't do this earlier since we didn't know when it would be processed
            end_point(lbnd->ELedge,lbnd->ELpm,v); // set the endpoint of the left HalfEdge to be this vector
            end_point(rbnd->ELedge,rbnd->ELpm,v); // set the endpoint of the right HalfEdge to be this vector
            delete_el(lbnd); // mark the lowest HE for deletion - can't delete yet because there might be pointers to it in Hash Map
            delete_pq(rbnd); // remove all vertex events to do with the  right HE
            delete_el(rbnd); // mark the right HE for deletion - can't delete yet because there might be pointers to it in Hash Map
            pm = le; // set the pm variable to zero

            // if the site to the left of the event is higher than the Site
            // to the right of it, then swap them and set the 'pm' variable to 1
            if (bot->coord.y > top->coord.y) {
                temp = bot;
                bot = top;
                top = temp;
                pm = re;
            }
            e = bisect(bot, top); // create an VoronoiEdge (or line) that is between the two Sites. This creates
            // the formula of the line, and assigns a line number to it
            bisector = create_he(e, pm); // create a HE from the VoronoiEdge 'e', and make it point to that edge with its ELedge field
            insert_el(llbnd, bisector); // insert the new bisector to the right of the left HE
            end_point(e, re-pm, v); // set one endpoint to the new edge to be the vector point 'v'.
            // If the site to the left of this bisector is higher than the right
            // Site, then this endpoint is put in position 0; otherwise in pos 1
            deref(v); // delete the vector 'v'

            // if left HE and the new bisector don't intersect, then delete the left HE, and reinsert it
            if((p = intersect(llbnd, bisector)) != (struct Site *) NULL) {
                delete_pq(llbnd);
                insert_pq(llbnd, p, dist(p,bot));
            }

            // if right HE and the new bisector don't intersect, then reinsert it
            if ((p = intersect(bisector, rrbnd)) != (struct Site *) NULL) {
                insert_pq(bisector, p, dist(p,bot));
            }
        } else {
            break;
        }
    }

    for(lbnd=right_el(kELLeftEnd); lbnd != kELRightEnd; lbnd=right_el(lbnd)) {
        e = lbnd->ELedge;
        clip_line(e);
    }

    clean_up();

    return true;
}


int scomp(const void *p1,const void *p2) {
    struct VoronoiPoint *s1 = (VoronoiPoint*)p1, *s2=(VoronoiPoint*)p2;

    if(s1->y < s2->y) {
        return(-1);
    }
    if(s1->y > s2->y) {
        return(1);
    }
    if(s1->x < s2->x) {
        return(-1);
    }
    if(s1->x > s2->x) {
        return(1);
    }

    return(0);
}

/* return a single in-storage site */
struct Site * Fortune::next_one() {
    struct Site *s;
    if(kSiteIdx < kNSites) {
        s = &kSites[kSiteIdx];
        kSiteIdx += 1;
        return(s);
    } else {
        return ((struct Site *) NULL);
    }
}
