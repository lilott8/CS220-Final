#include "../Headers/spc.h"
#include <vector>
#include <boost/polygon/voronoi_diagram.hpp>

using namespace Flow;

SPC::SPC(){
    kBinaryTree = BinaryTree();
}

SPC::SPC(vector<boost::polygon::voronoi_diagram<double>::cell_type> c) {
    kCells = c;
    kBinaryTree = BinaryTree();
}

SPC::~SPC(){}


void SPC::start() {

}

void SPC::set_cells(vector<boost::polygon::voronoi_cell<double>> c) {
    kCells = c;
}