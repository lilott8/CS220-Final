#include "../Headers/controller.h"
#include "../Headers/vnode.h"
#include "../Headers/problem_object.h"
#include "../Headers/claim.h"
#include "../Headers/map.h"
#include "../Headers/vedge.h"

using namespace Flow;
using namespace Utilities;
using namespace std;

vector<vector<VNode*>> kMap;

Map::Map() {
    kHeight = kWidth = kDefaultSize;
    kIsEuclidean = false;
    initialize_map();
}

Map::~Map() {

}

Map::Map(ProblemObject *po) {
    kHeight = po->get_height();
    kWidth = po->get_width();
    initialize_map();
    set_blockages(po->get_blockers());
    set_pins(po->get_connections());
    generate_hanan_grid();
}

/**
* Add the blockages to the map
*/
void Map::set_blockages(vector<Blocker> b) {
    int height;
    int width;
    Point start;

    // For all the blockages in our problem object
    for(int x = 0;x < (int)b.size();x++) {
        height = b.at(x).height;
        width = b.at(x).width;
        start = b.at(x).location;

        // Check to see if we can place the blockage!
        if(width + start.x > kWidth || height + start.y > kHeight) {
            claim("M/set_blockages: We cannot create the map: invalid blockage size", kError);
        }

        // Place the blockage first
        int h = 0;
        int w = 0;
        while (w < width) {
            while (h < height) {
                kMap.at(start.x + w).at(start.y + h)->set_type(VNode::Type::BLOCKED);
                h++;
            }
            h = 0;
            w++;
        }

        /**
        * Now we can check to see if we can add the corner places
        */
        // up and left from beginning of blockage (top left corner)
        if(start.x - 1 > 0 && start.y - 1 > 0) {
            if(kMap.at(start.x).at(start.y)->get_type() == VNode::NONE) {
                kMap.at(start.x).at(start.y)->set_type(VNode::CORNER);
                kCorners.insert(kMap.at(start.x-1).at(start.y-1));
            }
        }

        // width of blockage, y (top right corner)
        if((start.x + (width-1)) + 1 < kWidth && start.y - 1 > 0) {
            if(kMap.at(start.x+(width-1)+1).at(start.y-1)->get_type() == VNode::NONE) {
                kMap.at(start.x+(width-1)+1).at(start.y-1)->set_type(VNode::CORNER);
                kCorners.insert(kMap.at(start.x+(width-1)+1).at(start.y-1));
            }
        }

        // start x, height of blockage (bottom left corner)
        if(start.x - 1 > 0 && (start.y + height) + 1 < kHeight) {
            if(kMap.at(start.x-1).at(start.y+(height-1)+1)->get_type() == VNode::NONE) {
                kMap.at(start.x-1).at(start.y+(height-1)+1)->set_type(VNode::CORNER);
                kCorners.insert(kMap.at(start.x-1).at(start.y+(height-1)+1));
            }
        }

        // width of blockage, height of blockage (bottom right corner)
        if((start.x + width) + 1 < kWidth && (start.y + height) + 1 < kHeight) {
            if(kMap.at(start.x+(width-1)+1).at(start.y+(height-1)+1)->get_type() == VNode::NONE) {
                kMap.at(start.x + (width - 1) + 1).at(start.y + (height - 1) + 1)->set_type(VNode::CORNER);
                kCorners.insert(kMap.at(start.x + (width - 1) + 1).at(start.y + (height - 1) + 1));
            }
        }
    }
}

/**
* add the pins to the map
*/
void Map::set_pins(vector<Connection> c) {
    for(int x = 0;x < (int)c.size(); x++) {
        // Declare the pin(s)
        if(kMap.at(c.at(x).source.x).at(c.at(x).source.y)->get_type() != VNode::Type::BLOCKED) {
            kMap.at(c.at(x).source.x).at(c.at(x).source.y)->set_type(VNode::Type::PIN);
            kPins.insert(kMap.at(c.at(x).source.x).at(c.at(x).source.y));
        } else {
            claim("M/sp: We couldn't place: " + kMap.at(c.at(x).source.x).at(c.at(x).source.y)->coords_to_string()
                    + " something was already there!", kWarning);
        }

        if(kMap.at(c.at(x).sink.x).at(c.at(x).sink.y)->get_type() != VNode::Type::BLOCKED) {
            kMap.at(c.at(x).sink.x).at(c.at(x).sink.y)->set_type(VNode::Type::PIN);
            kPins.insert(kMap.at(c.at(x).sink.x).at(c.at(x).sink.y));
        } else {
            claim("M/sp: We couldn't place: " + kMap.at(c.at(x).source.x).at(c.at(x).source.y)->coords_to_string()
                    + " something was already there!", kWarning);
        }
    }
}

/**
* get the pins that are required for the map object
*/
std::set<VNode*> Map::get_pins() {
    return kPins;
}

std::set<VNode*> Map::get_corners() {
    return kCorners;
}

/**
* return the map object
*/
vector<vector<VNode*>> Map::get_map() {
    return kMap;
}

/**
* get the max x
*/
int Map::get_x() {
    return kHeight;
}

/**
* get the max y
*/
int Map::get_y() {
    return kWidth;
}

vector<MapRoute*> Map::get_routes() {
    return kRoutes;
}

/**
* begin building our map object from the problem object, first initialize the vectors
*/
void Map::initialize_map() {
    for(int y = 0; y < kHeight; y++) {
        vector<VNode*> temp_row;
        for(int x = 0; x < kWidth; x++) {
            /**
            * TODO: it should be new Vnode(x,y,0);!?!?!?!?
            */
            temp_row.push_back(new VNode(y,x,Controller::calculate_distance(y, x)));
        }
        kMap.push_back(temp_row);
    }
}

/**
* print the map to std out
*/
void Map::print_map() {
    string output = "\t";
    for(int x = 0; x < kHeight; x++) {
        output += to_string(x) + "\t";
    }

    int size = (int) output.size();
    output += "\n\t";

    for(int x = 0; x<size;x++) {
        output += "-";
    }

    output += "\n";

    for(int y = 0; y < kHeight; y++) {
        if(y < 10) {
            output += to_string(y) + " |\t";
        } else {
            output += to_string(y) + "|\t";
        }
        for(int x = 0; x < kWidth; x++) {
            if(kMap.at(x).at(y)->get_type() == VNode::Type::BLOCKED ||
                    kMap.at(x).at(y)->get_type() == VNode::Type::PIN ||
                    kMap.at(x).at(y)->get_type() == VNode::Type::PATH ||
                    kMap.at(x).at(y)->get_type() == VNode::Type::EDGE ||
                    kMap.at(x).at(y)->get_type() == VNode::Type::STEINER ||
                    kMap.at(x).at(y)->get_type() == VNode::Type::VORONOI ||
                    kMap.at(x).at(y)->get_type() == VNode::Type::CORNER) {
                output += VNode::type_to_string(kMap.at(x).at(y)->get_type()) + "\t";
            } else {
                if(kMap.at(x).at(y)->get_output() == 0) {
                    output += " \t";
                } else {
                    output += to_string(kMap.at(x).at(y)->get_output()) + "\t";
                }
            }
        }
        output += "\n";
    }
    claim("\n" + output, kDebug);
}

/**
* Debugging method for projecting the voronoi solution onto the map
*/
void Map::draw_voronoi_edges(vector<VEdge*> edges) {
    draw_bresenham_lines(edges);
}

/**
* A trivial implementation of Bresenham's line algorithm
*/
void Map::draw_bresenham_lines(vector<VEdge*> edges) {
    for(VEdge *edge : edges) {
        float x1, y1, x2, y2;
        x1 = edge->kStart->get_x();
        x2 = edge->kEnd->get_x();
        y1 = edge->kStart->get_y();
        y2 = edge->kEnd->get_y();
        //claim("Drawing Line: (" + to_string(x1) + ", " + to_string(y1) + ")\t->\t(" + to_string(x2) + ", " + to_string(y2) + ")", kDebug);
        const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
        if(steep) {
            mapswap(x1, y1);
            mapswap(x2, y2);
        }

        if(x1 > x2) {
            mapswap(x1, x2);
            mapswap(y1, y2);
        }

        const float dx = x2 - x1;
        const float dy = fabs(y2 - y1);

        float error = dx / 2.0f;
        const int ystep = (y1 < y2) ? 1 : -1;
        int y = (int)y1;

        const int maxX = (int)x2;

        for(int x=(int)x1; x<=maxX; x++) {
            if(steep) {
                if(kMap.at(y).at(x)->get_type() == VNode::Type::NONE) {
                    kMap.at(y).at(x)->set_type(VNode::Type::EDGE);
                }
            } else {
                if(kMap.at(x).at(y)->get_type() == VNode::Type::NONE) {
                    kMap.at(x).at(y)->set_type(VNode::Type::EDGE);
                }
            }

            error -= dy;
            if(error < 0) {
                y += ystep;
                error += dx;
            }
        }
    }
}

void Map::set(VNode* node) {
    if(kMap.at(node->get_x()).at(node->get_y())->get_type() == VNode::Type::NONE) {
        //claim("M/set: Setting: " + node->vnode_to_string() + " to: " + node->type_to_string(node->get_type()), kDebug);
        kMap.at(node->get_x()).at(node->get_y())->set_type(node->get_type());
    }
}

void Map::generate_hanan_grid() {
    for(auto pin : kPins) {
        for(auto c : kCorners) {
            if(pin != c) {
                kRoutes.push_back(new MapRoute(pin, c));
            }
        }
    }
}

VNode* Map::get_closest_node(VNode* n) {
    return *(kPins.lower_bound(n));
}
