#ifndef _FORTUNE_H_
#define _FORTUNE_H_

#pragma once

#include "algorithm.h"

using namespace Utilities;

namespace Flow {
    class Fortune : public Algorithm {
    public:
        Fortune();
        ~Fortune();
        void start();
    private:
    };
}

#endif