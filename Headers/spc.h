#ifndef _SPC_H_
#define _SPC_H_

#pragma once

#include <vector>
#include <boost/polygon/voronoi_diagram.hpp>
#include "vnode.h"

using namespace Utilities;

namespace Flow {
    class SPC {
    public:
        SPC();
        SPC(vector<boost::polygon::voronoi_diagram<double>::cell_type>);
        ~SPC();

        void start();

        void set_cells(vector<boost::polygon::voronoi_diagram<double>::cell_type>);
    private:
        vector<boost::polygon::voronoi_diagram<double>::cell_type> kCells;
    };
}

#endif