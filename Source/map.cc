#include "../Headers/vnode.h"
#include "../Headers/problem_object.h"
#include "../Headers/claim.h"
#include "../Headers/map.h"

using namespace Flow;
using namespace Utilities;

Map::Map() {
    kHeight = kWidth = kDefaultSize;
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
        /**
        * TODO: add error handling for placing of pins
        */
        // Declare the pin(s)
        kMap.at(c.at(x).source.x).at(c.at(x).source.y)->set_type(VNode::Type::PIN);
        kPins.push(kMap.at(c.at(x).source.x).at(c.at(x).source.y));

        kMap.at(c.at(x).sink.x).at(c.at(x).sink.y)->set_type(VNode::Type::PIN);
        kPins.push(kMap.at(c.at(x).sink.x).at(c.at(x).sink.y));
    }
}

priority_queue<VNode*, vector<VNode*>, CloserToOrigin> Map::get_pins() {
    return kPins;
}

void Map::initialize_map() {
    for(int y = 0; y < kHeight; y++) {
        vector<VNode*> temp_row;
        for(int x = 0; x < kWidth; x++) {
            temp_row.push_back(new VNode(x,y,0));
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
                output += to_string(kMap.at(x).at(y)->get_output()) + "\t";
            }
        }
        output += "\n";
    }
    claim("\n" + output, kDebug);
}