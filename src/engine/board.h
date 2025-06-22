#pragma once

#include <string_view>
#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <piece_utils.h>

class Board {
    public:
        explicit Board(const std::string& fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        uint8_t getPieceAt(int index) const;
    private:
        void validateFen();
        void initializeFromFen();

        uint8_t boardArr_m[64] = {0};
        std::string fen_m;
};


