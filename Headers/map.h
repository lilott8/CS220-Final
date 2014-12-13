#ifndef _MAP_H_
#define _MAP_H_

#include "vnode.h"
#include "problem_object.h"
#include <queue>
#include "comparators.h"

using namespace Utilities;
using namespace std;

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
        priority_queue<VNode*, vector<VNode*>, CloserToOrigin> get_pins();

    private:
        int kHeight;
        int kWidth;
        int kDefaultSize = 10;
        priority_queue<VNode*, vector<VNode*>, CloserToOrigin> kPins;
        vector<vector<VNode*>> kMap;

        void initialize_map();
        void set_pins(vector<Connection>);
        void set_blockages(vector<Blocker>);
    };
}

#endif