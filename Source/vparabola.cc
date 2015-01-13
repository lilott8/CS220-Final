#include "../Headers/VParabola.h"

using namespace Flow;

VParabola::VParabola()
{
    kSite	= 0;
    kIsLeaf	= false;
    kCEvent	= 0;
    kEdge	= 0;
    kParent	= 0;
}

VParabola::VParabola(VNode* s)
{
    kSite	= s;
    kIsLeaf	= true;
    kCEvent	= 0;
    kEdge	= 0;
    kParent	= 0;
}

/*
	Tree operations (described in the header file)
*/

VParabola* VParabola::get_left(VParabola * p)
{
    return get_left_child(get_left_parent(p));
}


VParabola* VParabola::get_right(VParabola * p)
{
    return get_right_child(get_right_parent(p));
}

VParabola* VParabola::get_left_parent	(VParabola * p)
{
    VParabola * par		= p->kParent;
    VParabola * pLast	= p;
    while(par->left() == pLast)
    {
        if(!par->kParent) return 0;
        pLast = par;
        par = par->kParent;
    }
    return par;
}

VParabola* VParabola::get_right_parent(VParabola * p)
{
    VParabola * par		= p->kParent;
    VParabola * pLast	= p;
    while(par->right() == pLast)
    {
        if(!par->kParent) return 0;
        pLast = par; par = par->kParent;
    }
    return par;
}

VParabola* VParabola::get_left_child(VParabola * p)
{
    if(!p) return 0;
    VParabola * par = p->left();
    while(!par->kIsLeaf) par = par->right();
    return par;
}

VParabola* VParabola::get_right_child(VParabola * p)
{
    if(!p) return 0;
    VParabola * par = p->right();
    while(!par->kIsLeaf) par = par->left();
    return par;
}

void VParabola::set_left(VParabola* p) {
    kLeft = p;
    p->kParent = this;
}

void VParabola::set_right(VParabola* p) {
    kRight = p;
    p->kParent = this;
}

VParabola* VParabola::left() {
    return kLeft;
}

VParabola* VParabola::right() {
    return kRight;
}
