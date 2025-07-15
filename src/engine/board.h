#pragma once

#include "chess.h"
#include <cstdint>
#include <string>
#include <format>

#include <piece_utils.h>

/*
0x88 Board Representation: 

70 71 72 73 74 75 76 77 | 78 79 7A 7B 7C 7D 7E 7F
60 61 62 63 64 65 66 67 | 68 69 6A 6B 6C 6D 6E 6F
50 51 52 53 54 55 56 57 | 58 59 5A 5B 5C 5D 5E 5F
40 41 42 43 44 45 46 47 | 48 49 4A 4B 4C 4D 4E 4F
30 31 32 33 34 35 36 37 | 38 39 3A 3B 3C 3D 3E 3F
20 21 22 23 24 25 26 27 | 28 29 2A 2B 2C 2D 2E 2F
10 11 12 13 14 15 16 17 | 18 19 1A 1B 1C 1D 1E 1F
00 01 02 03 04 05 06 07 | 08 09 0A 0B 0C 0D 0E 0F
*/

class Board {
    public:
        explicit Board(const std::string& fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        bool isEmptyAt(int index) const;
        bool isEmptyAt(chess::Tile tile) const;
        uint8_t getPieceAt(int Index) const;
        uint8_t getPieceAt(chess::Tile tile) const;
        void makeMove(chess::Tile start, chess::Tile end);
        
    private:
        void validateFen();
        void initializeFromFen();

        // 0x88 style board representation
        uint8_t boardArr_[128] = {0};
        std::string fen_;
};


