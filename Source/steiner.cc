#include <controller.h>
#include "../Headers/map.h"
#include "../Headers/steiner.h"

using namespace Flow;
using namespace FlowAlgorithms;

Steiner::Steiner() {
}

Steiner::Steiner(set<VNode*> v) {
    kAllVertices = v;
}

Steiner::~Steiner() {
}


void Steiner::start() {
    claim("S/start: Size of queue: " + to_string(kAllVertices.size()), kDebug);
    generate_steiner_intersections();
}

set<VNode*> Steiner::get_steiner_points() {
    return kSteinerPoints;
}

set<VEdge*> Steiner::get_steiner_edges() {
    return kSteinerEdges;
}

void Steiner::generate_steiner_intersections() {
    //VNode *node1;
    //VNode *node2;

    for(VNode* outer : kAllVertices) {
        for(VNode* inner : kAllVertices) {
         if(outer == inner) {
             continue;
         }

            if(outer->get_x() == inner->get_x()) {
                // Calculate the midpoint
                int midpoint = (outer->get_y() + inner->get_y()) / 2;

                // Toggle the point on the map that needs to become a steiner point
                Map::get_map().at(outer->get_x()).at(midpoint)->set_type(VNode::Type::STEINER);
                // Add the steiner point to our array of candidate vertices
                kSteinerPoints.insert(Map::get_map().at(outer->get_x()).at(midpoint));

                // Save point x -> midpoint
                kSteinerEdges.insert(new VEdge(
                        outer, Map::get_map().at(outer->get_x()).at(midpoint)
                ));

                // save midpoint -> point y
                kSteinerEdges.insert(new VEdge(
                        Map::get_map().at(inner->get_x()).at(midpoint), inner
                ));
            } else if(outer->get_y() == inner->get_y()) {
                // Calculate the midpoint
                int midpoint = (outer->get_x() + inner->get_x()) / 2;

                // Toggle the point on the map that needs to become a steiner point
                Map::get_map().at(midpoint).at(outer->get_y())->set_type(VNode::Type::STEINER);
                // Add the steiner point to our array of candidate vertices
                kSteinerPoints.insert(Map::get_map().at(midpoint).at(outer->get_y()));

                // Save point x -> midpoint
                kSteinerEdges.insert(new VEdge(
                        outer, Map::get_map().at(midpoint).at(outer->get_y())
                ));
                // Save midpoint -> point y
                kSteinerEdges.insert(new VEdge(
                        Map::get_map().at(midpoint).at(inner->get_y()), inner
                ));
            } else {
                // Generate the x/y of the midpoint
                int mx, my;
                mx = (outer->get_x() + inner->get_x()) / 2;
                my = (outer->get_y() + inner->get_y()) / 2;

                // Toggle the point on the map that needs to become a steiner point
                Map::get_map().at(mx).at(my)->set_type(VNode::Type::STEINER);

                // Add the steiner point to our array of candidate vertices
                kSteinerPoints.insert(Map::get_map().at(mx).at(my));

                // Create the vedges for our new steiner points
                kSteinerEdges.insert(new VEdge(
                        outer, Map::get_map().at(mx).at(my)
                ));
                // Create the vedges for our new steiner points
                kSteinerEdges.insert(new VEdge(
                        Map::get_map().at(mx).at(my), inner
                ));
            }


        }
    }
}


/**
* http://stackoverflow.com/questions/12934213/how-to-find-out-geometric-median
* http://www.mathblog.dk/project-euler-143-investigating-the-torricelli-point-of-a-triangle/
*/
void Steiner::generate_steiner_points() {
    int y = 0;

    for(int x = 0; x<(int)kTriangles.size();x++) {
        if (verify_triangle_angles(kTriangles.at(x))) {
            kSteinerPoints.insert(manhattan_geometric_mean(kTriangles.at(x)));
            y++;
        } else {
            //claim("Angle is greater than 120!", kDebug);
            //claim(st->triangle_to_string(), kDebug);
            //claim("=======================================", kDebug);
        }
    }
    claim("S/generate_steiner_points: " + to_string(y) + "/" + to_string(kTriangles.size()) + " of triangles have steiner points", kDebug);
    claim("S/generate_steiner_points: Total number of Steiner Points: " + to_string(kSteinerPoints.size()), kDebug);
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
        //data[index] = kAllVertices.at(i);
        //generate_triangles(data, i+1, end, index+1, combo);
    }

}

bool Steiner::verify_triangle_angles(SteinerTriangle* t) {
    double angle1, angle2, angle3;
    double total = 0;

    int largest_length = 0;

    double dist1 = calculate_euclidean_distance(t->p1, t->p2);
    double dist2 = calculate_euclidean_distance(t->p2, t->p3);
    double dist3 = calculate_euclidean_distance(t->p1, t->p3);

    if(dist1 > dist2 && dist1 > dist3) {
        angle1 = find_bigger_angle(dist1, dist2, dist3);
        angle2 = find_other_angle(angle1, dist1, dist2);
        angle3 = find_other_angle(angle1, dist1, dist3);

        total = angle1 + angle2 + angle3;
        if(total < 180) {
            angle1 = 180 - angle1;
        }
    } else if(dist2 > dist3 && dist2 > dist1) {
        angle2 = find_bigger_angle(dist2, dist1, dist3);
        angle1 = find_other_angle(angle2, dist2, dist1);
        angle3 = find_other_angle(angle2, dist2, dist3);

        total = angle1 + angle2 + angle3;
        if(total < 180) {
            angle2 = 180 - angle2;
        }
    } else {
        angle3 = find_bigger_angle(dist3, dist1, dist2);
        angle1 = find_other_angle(angle3, dist3, dist1);
        angle2 = find_other_angle(angle3, dist3, dist2);

        total = angle1 + angle2 + angle3;
        if(total < 180) {
            angle3 = 180 - angle3;
        }
    }
    claim("S/verify_triangle_angles: Size of angles: " + to_string(angle1) + "\t" + to_string(angle2) + "\t" + to_string(angle3), kDebug);

    return (angle1 > 120 || angle2 > 120 || angle3 > 120);
}

VNode* Steiner::manhattan_geometric_mean(SteinerTriangle* tri) {
    int ax = (tri->p1->get_x() + tri->p2->get_x() + tri->p3->get_x())/3;
    int ay = (tri->p1->get_y() + tri->p2->get_y() + tri->p3->get_y())/3;
    VNode* v = new VNode(ax, ay);
    v->set_type(VNode::Type::STEINER);
    return v;
}

double Steiner::euclidean_geometric_mean() {
    
}

double Steiner::calculate_euclidean_distance(VNode* a, VNode* b) {
    double order1, order2;
    order1 = (a->get_x() - b->get_x()) * (a->get_x() - b->get_x());
    order2 = (a->get_y() - b->get_y()) * (a->get_y() - b->get_y());
    // http://stackoverflow.com/questions/9695329/c-how-to-round-a-double-to-an-int
    return sqrt(order1 + order2);
}

double Steiner::find_other_angle(double l_a, double l_d, double s_d) {
    double angle = s_d * sin(l_a * PI/180);
    return asin((angle/l_d))*180 / PI;
}

double Steiner::find_bigger_angle(double l_d, double s_d_1, double s_d_2) {
    double angle = pow(s_d_1,2) + pow(s_d_2, 2) - pow(l_d, 2);
    angle = fabs(angle/(2*s_d_1 * s_d_2));
    return acos(angle) * 180 / PI;
}