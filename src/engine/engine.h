#pragma once

#include "engine/moveGen.h"
#include "engine/board.h"
#include <limits>

class Engine {
    public:
        
    private:
        void minimax();


        // Infinity
        static constexpr float inf_ = std::numeric_limits<float>::infinity();

};