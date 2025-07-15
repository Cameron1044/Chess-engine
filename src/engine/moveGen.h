#pragma once

#include "chess.h"
#include <vector>
#include <array>
#include <iostream>
#include "engine/board.h"

class MoveGen {
    public:
        void genLegalMovesAtTile(chess::Tile tile, const Board& board, std::vector<chess::Tile>& legalMoves);
        std::vector<int> genLegalMovesAt(int index, const Board& board);

    private:
        void genLateral(int index, std::vector<int>& legalmoves, const Board& board);
        void genDiagonal(int index, std::vector<int>& legalmoves, const Board& board);
        //void genMovesFromMoveArray(int index, std::vector<int>& legalmoves, const Board& board, const std::vector<int>& moveArray);

        void genPawn(int index, std::vector<int>& legalmoves, const Board& board);
        void genKnight(int index, std::vector<int>& legalmoves, const Board& board);
        void genBishop(int index, std::vector<int>& legalmoves, const Board& board);
        void genRook(int index, std::vector<int>& legalmoves, const Board& board);
        void genQueen(int index, std::vector<int>& legalmoves, const Board& board);
        void genKing(int index, std::vector<int>& legalmoves, const Board& board);
};