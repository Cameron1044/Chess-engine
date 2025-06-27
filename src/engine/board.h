#pragma once

#include <cstdint>
#include <string>

#include <piece_utils.h>

class Board {
    public:
        explicit Board(const std::string& fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        bool isEmpty(int file, int rank) const;
        uint8_t getPieceAt(int file, int rank) const;
        void makeMove(int startFile, int startRank, int endFile, int endRank);
        
    private:
        int fileRankToIndex(int file, int rank) const;
        void validateFen();
        void initializeFromFen();

        uint8_t boardArr_m[64] = {0};
        std::string fen_m;
};


