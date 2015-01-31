#include "../Headers/vnode.h"
#include "../Headers/problem_object.h"
#include "../Headers/claim.h"
#include "../Headers/map.h"
#include "../Headers/vedge.h"

using namespace Flow;
using namespace Utilities;

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
}

/**
* A trivial implementation of Bresenham's line algorithm
*/
void Map::set_blockages(vector<Blocker> b) {
    int height;
    int width;
    Point start;

    for(int x = 0;x < (int)b.size();x++) {
        height = b.at(x).height;
        width = b.at(x).width;
        start = b.at(x).location;

        if(width + start.x > kWidth || height + start.y > kHeight) {
            claim("M/set_blockages: We cannot create the map: invalid blockage size", kError);
        }

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
    }
}

void Map::set_pins(vector<Connection> c) {
    for(int x = 0;x < (int)c.size(); x++) {
        // Declare the pin(s)
        if(kMap.at(c.at(x).source.x).at(c.at(x).source.y)->get_type() == VNode::Type::NONE) {
            kMap.at(c.at(x).source.x).at(c.at(x).source.y)->set_type(VNode::Type::PIN);
            kPins.push(kMap.at(c.at(x).source.x).at(c.at(x).source.y));
        } else {
            claim("M/sp: We couldn't place: " + kMap.at(c.at(x).source.x).at(c.at(x).source.y)->coords_to_string()
                    + " something was already there!", kWarning);
        }

        if(kMap.at(c.at(x).sink.x).at(c.at(x).sink.y)->get_type() == VNode::Type::NONE) {
            kMap.at(c.at(x).sink.x).at(c.at(x).sink.y)->set_type(VNode::Type::PIN);
            kPins.push(kMap.at(c.at(x).sink.x).at(c.at(x).sink.y));
        } else {
            claim("M/sp: We couldn't place: " + kMap.at(c.at(x).source.x).at(c.at(x).source.y)->coords_to_string()
                    + " something was already there!", kWarning);
        }
    }
}

priority_queue<VNode*, vector<VNode*>, CloserToOrigin> Map::get_pins() {
    return kPins;
}

void Map::initialize_map() {
    for(int y = 0; y < kHeight; y++) {
        vector<VNode*> temp_row;
        for(int x = 0; x < kWidth; x++) {
            /**
            * TODO: it should be new Vnode(x,y,0);!?!?!?!?
            */
            temp_row.push_back(new VNode(y,x,0));
        }
        this->kMap.push_back(temp_row);
    }
}

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
                    kMap.at(x).at(y)->get_type() == VNode::Type::STEINER) {
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

int Map::get_x() {
    return kHeight;
}

int Map::get_y() {
    return kWidth;
}

void Map::draw_voronoi_edges(vector<VEdge*> edges) {
    if(!kIsEuclidean) {
        draw_xiaolin_wu_lines(edges);
    } else {
        draw_bresenham_lines(edges);
    }
    claim("M/draw_voronoi_edges: still using bresenham lines!", kNote);
    draw_bresenham_lines(edges);
}

void Map::draw_bresenham_lines(vector<VEdge*> edges) {
    for(VEdge *edge : edges) {
        float x1, y1, x2, y2;
        x1 = edge->kStart->get_x();
        x2 = edge->kEnd->get_x();
        y1 = edge->kStart->get_y();
        y2 = edge->kEnd->get_y();
        const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
        if(steep) {
            std::swap(x1, y1);
            std::swap(x2, y2);
        }

        if(x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        const float dx = x2 - x1;
        const float dy = fabs(y2 - y1);

        float error = dx / 2.0f;
        const int ystep = (y1 < y2) ? 1 : -1;
        int y = (int)y1;

        const int maxX = (int)x2;

        for(int x=(int)x1; x<=maxX; x++) {
            if(steep) {
                kMap.at(y).at(x)->set_type(VNode::Type::EDGE);
            } else {
                kMap.at(x).at(y)->set_type(VNode::Type::EDGE);
            }

            error -= dy;
            if(error < 0) {
                y += ystep;
                error += dx;
            }
        }
    }
}


/**
* http://rosettacode.org/wiki/Xiaolin_Wu%27s_line_algorithm#C
*/

void Map::draw_xiaolin_wu_lines(vector<VEdge *> edges) {/*
    double dx = (double)x2 - (double)x1;
    double dy = (double)y2 - (double)y1;
    if ( fabs(dx) > fabs(dy) ) {
        if ( x2 < x1 ) {
            swap_(x1, x2);
            swap_(y1, y2);
        }
        double gradient = dy / dx;
        double xend = round(x1);
        double yend = y1 + gradient*(xend - x1);
        double xgap = rfpart_(x1 + 0.5);
        int xpxl1 = xend;
        int ypxl1 = round(yend);
        plot_(xpxl1, ypxl1, rfpart_(yend)*xgap);
        plot_(xpxl1, ypxl1+1, fpart_(yend)*xgap);
        double intery = yend + gradient;

        xend = round(x2);
        yend = y2 + gradient*(xend - x2);
        xgap = fpart_(x2+0.5);
        int xpxl2 = xend;
        int ypxl2 = round(yend);
        plot_(xpxl2, ypxl2, rfpart_(yend) * xgap);
        plot_(xpxl2, ypxl2 + 1, fpart_(yend) * xgap);

        int x;
        for(x=xpxl1+1; x <= (xpxl2-1); x++) {
            plot_(x, round(intery), rfpart_(intery));
            plot_(x, round(intery) + 1, fpart_(intery));
            intery += gradient;
        }
    } else {
        if ( y2 < y1 ) {
            swap_(x1, x2);
            swap_(y1, y2);
        }
        double gradient = dx / dy;
        double yend = round(y1);
        double xend = x1 + gradient*(yend - y1);
        double ygap = rfpart_(y1 + 0.5);
        int ypxl1 = yend;
        int xpxl1 = round(xend);
        plot_(xpxl1, ypxl1, rfpart_(xend)*ygap);
        plot_(xpxl1, ypxl1+1, fpart_(xend)*ygap);
        double interx = xend + gradient;

        yend = round(y2);
        xend = x2 + gradient*(yend - y2);
        ygap = fpart_(y2+0.5);
        int ypxl2 = yend;
        int xpxl2 = round(xend);
        plot_(xpxl2, ypxl2, rfpart_(xend) * ygap);
        plot_(xpxl2, ypxl2 + 1, fpart_(xend) * ygap);

        int y;
        for(y=ypxl1+1; y <= (ypxl2-1); y++) {
            plot_(round(interx), y, rfpart_(interx));
            plot_(round(interx) + 1, y, fpart_(interx));
            interx += gradient;
        }
    }
    */
}
