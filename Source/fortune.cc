#include "../Headers/claim.h"
#include "../Headers/fortune.h"
#include "../Headers/vevent.h"
#include "../Headers/arc.h"

using namespace Flow;

Fortune::Fortune() {
    kRoot = 0;
    kWidth = 10;
    kHeight = 10;
}

Fortune::Fortune(int h, int w) {
    kRoot = 0;
    kWidth = w;
    kHeight = h;
}

Fortune::~Fortune() {

}

void Fortune::start(priority_queue<VNode*, vector<VNode*>, CloserToOrigin> queue) {
    FlowAlgorithm::start(queue);
    claim("F/start: Starting Fortunes!", kDebug);

    /**
    * Call get_edges from this method to set our edges straight.
    */
    list<VNode*> temp;
    while(!queue.empty()) {
        temp.push_front(queue.top());
        queue.pop();
    }
    list<VEdge*> edges = get_edges(temp);

    claim("F/start: We have: " + to_string(edges.size()) + " edges", kNote);

}

list<VEdge*> Fortune::get_edges(list<VNode*> v) {
    kPlaces = v;
    kRoot = 0;

    for(list<VNode*>::iterator i = kPlaces.begin(); i!=kPlaces.end(); ++i) {
        // take the place and create an event from it
        queue.push(new VEvent(*i, true));
    }

    VEvent* e;
    while(!queue.empty())
    {
        e = queue.top();
        queue.pop();
        kLastY = e->point->get_y();
        
        if(kDeleted.find(e) != kDeleted.end()) {
            delete(e);
            kDeleted.erase(e);
            continue;
        }
        
        if(e->pe) {
            insert_parabola(e->point);
        } else {
            remove_parabola(e);
        }
        
        delete(e);
    }

    finish_edge(kRoot);

    for(list<VEdge*>::iterator i = kEdges.begin(); i != kEdges.end(); ++i) {
        if( (*i)->kNeighbor){
            (*i)->kStart = (*i)->kNeighbor->kEnd;
            delete (*i)->kNeighbor;
        }
    }

    return kEdges;
}

void Fortune::insert_parabola(VNode* p) {
    if(!kRoot){
        kRoot = new VParabola(p); return;
    }

    if(kRoot->kIsLeaf && kRoot->kSite->get_y() - p->get_y() < 1) {
        VNode* fp = kRoot->kSite;
        kRoot->kIsLeaf = false;
        kRoot->set_left( new VParabola(fp) );
        kRoot->set_right(new VParabola(p)  );
        VNode* s = new VNode((p->get_x() + fp->get_x())/2, kHeight);
        kPoints.push_back(s);

        if(p->get_x() > fp->get_x()){
            kRoot->kEdge = new VEdge(s, fp, p);
        } else {
            kRoot->kEdge = new VEdge(s, p, fp);
        }

        kEdges.push_back(kRoot->kEdge);
        return;
    }

    VParabola* par = get_parabola_by_x(p->get_x());

    if(par->kCEvent) {
        kDeleted.insert(par->kCEvent);
        par->kCEvent = 0;
    }

    VNode* kStart = new VNode(p->get_x(), get_y(par->kSite, p->get_x()));
    kPoints.push_back(kStart);

    VEdge* el = new VEdge(kStart, par->kSite, p);
    VEdge* er = new VEdge(kStart, p, par->kSite);

    el->kNeighbor = er;
    kEdges.push_back(el);

    par->kEdge = er;
    par->kIsLeaf = false;

    VParabola* p0 = new VParabola(par->kSite);
    VParabola* p1 = new VParabola(p);
    VParabola* p2 = new VParabola(par->kSite);

    par->set_right(p2);
    par->set_left(new VParabola());
    par->left()->kEdge = el;

    par->left()->set_left(p0);
    par->left()->set_right(p1);

    check_circle(p0);
    check_circle(p2);
}

void Fortune::remove_parabola(VEvent* e)
{
    VParabola* p1 = e->arch;

    VParabola* xl = VParabola::get_left_parent(p1);
    VParabola* xr = VParabola::get_right_parent(p1);

    VParabola* p0 = VParabola::get_left_child(xl);
    VParabola* p2 = VParabola::get_right_child(xr);

    if(p0 == p2) {
        claim("F/remove_parabola: We cannot have 2 equal parabolas!", kError);
    }

    if(p0->kCEvent) {
        kDeleted.insert(p0->kCEvent); p0->kCEvent = 0;
    }
    if(p2->kCEvent) {
        kDeleted.insert(p2->kCEvent); p2->kCEvent = 0;
    }

    VNode* p = new VNode(e->point->get_x(), get_y(p1->kSite, e->point->get_x()));
    kPoints.push_back(p);

    xl->kEdge->kEnd = p;
    xr->kEdge->kEnd = p;

    VParabola* higher;
    VParabola* par = p1;

    while(par != kRoot) {
        par = par->kParent;
        if(par == xl) higher = xl;
        if(par == xr) higher = xr;
    }
    higher->kEdge = new VEdge(p, p0->kSite, p2->kSite);
    kEdges.push_back(higher->kEdge);

    VParabola* gkParent = p1->kParent->kParent;

    if(p1->kParent->left() == p1) {
        if(gkParent->left()  == p1->kParent) gkParent->set_left ( p1->kParent->right() );
        if(gkParent->right() == p1->kParent) gkParent->set_right( p1->kParent->right() );
    } else {
        if(gkParent->left()  == p1->kParent) gkParent->set_left ( p1->kParent->left()  );
        if(gkParent->right() == p1->kParent) gkParent->set_right( p1->kParent->left()  );
    }

    delete p1->kParent;
    delete p1;

    check_circle(p0);
    check_circle(p2);
}

void Fortune::finish_edge(VParabola* n) {
    if(n->kIsLeaf) {delete n; return;}
    double mx;
    if(n->kEdge->kDirection->get_x() > 0.0) {
        mx = std::max(kWidth, n->kEdge->kStart->get_x() + 10);
    }
    else {
        mx = std::min(0, n->kEdge->kStart->get_x() - 10);
    }

    VNode* end = new VNode(mx, mx * n->kEdge->kF + n->kEdge->kG);
    n->kEdge->kEnd = end;
    kPoints.push_back(end);

    finish_edge(n->left() );
    finish_edge(n->right());
    delete n;
}

double Fortune::get_x_of_edge(VParabola* par, double y) {
    VParabola* left = VParabola::get_left_child(par);
    VParabola* right= VParabola::get_right_child(par);

    VNode* p = left->kSite;
    VNode* r = right->kSite;

    double dp = 2.0 * (p->get_y() - y);
    double a1 = 1.0 / dp;
    double b1 = -2.0 * p->get_x() / dp;
    double c1 = y + dp / 4 + p->get_x() * p->get_x() / dp;

    dp = 2.0 * (r->get_y() - y);
    double a2 = 1.0 / dp;
    double b2 = -2.0 * r->get_x()/dp;
    double c2 = kLastY + dp / 4 + r->get_x() * r->get_x() / dp;

    double a = a1 - a2;
    double b = b1 - b2;
    double c = c1 - c2;

    double disc = b*b - 4 * a * c;
    double x1 = (-b + std::sqrt(disc)) / (2*a);
    double x2 = (-b - std::sqrt(disc)) / (2*a);

    double ry;

    if(p->get_y() < r->get_y()){
        ry = std::max(x1, x2);
    } else {
        ry = std::min(x1, x2);
    }

    return ry;
}

VParabola* Fortune::get_parabola_by_x(double xx) {
    VParabola* par = kRoot;
    double x = 0.0;

    while(!par->kIsLeaf){
        x = get_x_of_edge(par, kLastY);
        if(x>xx) par = par->left();
        else par = par->right();
    }
    return par;
}

double Fortune::get_y(VNode* p, double x) {
    double dp = 2 * (p->get_y() - kLastY);
    double a1 = 1 / dp;
    double b1 = -2 * p->get_x() / dp;
    double c1 = kLastY + dp / 4 + p->get_x() * p->get_x() / dp;

    return(a1*x*x + b1*x + c1);
}

void Fortune::check_circle(VParabola* b) {
    VParabola* lp = VParabola::get_left_parent (b);
    VParabola* rp = VParabola::get_right_parent(b);

    VParabola* a  = VParabola::get_left_child (lp);
    VParabola* c  = VParabola::get_right_child(rp);

    if(!a || !c || a->kSite == c->kSite){
        return;
    }

    VNode* s = 0;
    s = get_edge_intersection(lp->kEdge, rp->kEdge);
    if(s == 0){
        return;
    }

    double dx = a->kSite->get_x() - s->get_x();
    double dy = a->kSite->get_y() - s->get_y();

    double d = std::sqrt( (dx * dx) + (dy * dy) );

    if(s->get_y() - d >= kLastY) {
        return;
    }

    VEvent* e = new VEvent(new VNode(s->get_x(), s->get_y() - d), false);
    kPoints.push_back(e->point);
    b->kCEvent = e;
    e->arch = b;
    queue.push(e);
}

VNode* Fortune::get_edge_intersection(VEdge* a, VEdge* b) {
    double x = (b->kG - a->kG) / (a->kF - b->kF);
    double y = a->kF * x + a->kG;

    if((x - a->kStart->get_x())/a->kDirection->get_x() < 0){
        return 0;
    }

    if((y - a->kStart->get_y())/a->kDirection->get_y() < 0){
        return 0;
    }

    if((x - b->kStart->get_x())/b->kDirection->get_x() < 0){
        return 0;
    }

    if((y - b->kStart->get_y())/b->kDirection->get_y() < 0){
        return 0;
    }

    VNode* p = new VNode(x, y);
    kPoints.push_back(p);
    return p;
}