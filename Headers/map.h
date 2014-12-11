#ifndef _MAP_H_
#define _MAP_H_

using namespace std;
using namespace Utilities;

#include "vnode.h"

namespace Flow {

    struct Route {
        Route(){};
        VNode pStart;
        VNode pStop;
    };

    class Map {
    public:
        Map();
        Map(ProblemObject*);
        ~Map();

        void print_map();

    private:
        int kHeight;
        int kWidth;
        int kDefaultSize = 10;
        vector<Route> kRoutes;
        vector<vector<VNode*>> kMap;

        void initialize_map();
        void set_pins(vector<Connection>);
        void set_blockages(vector<Blocker>);
    };
}

#endif