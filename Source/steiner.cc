#include "../Headers/map.h"
#include "../Headers/steiner.h"

using namespace Flow;
using namespace Algorithms;

Steiner::Steiner() {
}

Steiner::~Steiner() {
}


void Steiner::start() {
    claim("Size of queue: " + to_string(kAllVertices.size()), kDebug);
    generate_steiner_points();
}

void Steiner::set_vertices(vector<VNode*> v) {
    kAllVertices = v;
    // This is a fixed size because a triangle can only ever have 3 points
    VNode* data[3];
    generate_triangles(data, 0, kAllVertices.size()-1, 0, 3);
}


/**
* http://stackoverflow.com/questions/12934213/how-to-find-out-geometric-median
* http://www.mathblog.dk/project-euler-143-investigating-the-torricelli-point-of-a-triangle/
*/
void Steiner::generate_steiner_points() {
    int x = 0;

    for(int x = 0; x<(int)kTriangles.size();x++) {
        if (find_angle_size(kTriangles.at(x)->p1, kTriangles.at(x)->p2) <= 120 ||
                find_angle_size(kTriangles.at(x)->p1, kTriangles.at(x)->p3) <= 120 ||
                find_angle_size(kTriangles.at(x)->p2, kTriangles.at(x)->p3) <= 120) {
            kSteinerPoints.push_back(manhattan_geometric_mean(kTriangles.at(x)));
            x++;
        } else {
            //claim("Angle is greater than 120!", kDebug);
            //claim(st->triangle_to_string(), kDebug);
            //claim("=======================================", kDebug);
        }
    }
    claim(to_string(x) + "/" + to_string(kTriangles.size()) + " of triangles have steiner points", kDebug);
}


/**
* http://www.geeksforgeeks.org/print-all-possible-combinations-of-r-elements-in-a-given-array-of-size-n/
*/
void Steiner::generate_triangles(VNode* data[], int start, int end, int index, int combo) {
    // Base case, where we know we need to print create the triangle
    if(index == combo) {
        kTriangles.push_back(new SteinerTriangle(data[0], data[1], data[2]));
        //claim("Triangle: " + kTriangles.at(kTriangles.size()-1)->triangle_to_string(), kDebug);
        return;
    }

    // Otherwise, replace the index with all elements that could be paired.
    // "end-i+1 >= r-index" makes sure that including one element
    // at index will make a combination with remaining elements
    // at remaining positions
    for(int i=start; i<=end && end-i+1 >= combo - index; i++) {
        data[index] = kAllVertices.at(i);
        generate_triangles(data, i+1, end, index+1, combo);
    }

}

double Steiner::find_angle_size(VNode * a, VNode * b) {
    float angle = atan2(a->get_y() - b->get_y(), a->get_x() - b->get_x());
    claim(a->vnode_to_string() + "\t, " + b->vnode_to_string() + ": " + to_string(angle*180/M_PI), kDebug);
    return ((angle * 180) / M_PI);
}

VNode* Steiner::manhattan_geometric_mean(SteinerTriangle* tri) {
    int ax = (tri->p1->get_x() + tri->p2->get_x() + tri->p3->get_x())/3;
    int ay = (tri->p1->get_y() + tri->p2->get_y() + tri->p3->get_y())/3;
    return new VNode(ax, ay);
}

double Steiner::euclidean_geometric_mean() {
    
}