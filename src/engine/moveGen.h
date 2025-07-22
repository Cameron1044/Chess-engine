#pragma once

#include "chess.h"
#include <vector>
#include <array>
#include <iostream>
#include "engine/board.h"
#include "engine/move.h"

class MoveGen {
    public:
        MoveGen(Board board) : boardPtr_(std::make_shared<Board>(board)) {}
        MoveGen(BoardPtr boardPtr) : boardPtr_(boardPtr) {}

        void getPseudoMovesAtTile(chess::Tile tile, MoveList& legalMoves) const;
        MoveList getPseudoMovesAt(int from) const;

        // getters
        // MoveList getPseudoPawn(int from, MoveList& legalmoves) const;
        // MoveList getPseudoKnight(int from, MoveList& legalmoves) const;
        // MoveList getPseudoBishop(int from, MoveList& legalmoves) const;
        // MoveList getPseudoRook(int from, MoveList& legalmoves) const;
        // MoveList getPseudoQueen(int from, MoveList& legalmoves) const;
        // MoveList getPseudoKing(int from, MoveList& legalmoves) const;

    private:
        // appender helpers
        void addContinuousFromOffsetList(int from, MoveList& legalmoves, const std::array<int, 4>& offsetList) const;
        void addFromOffsetList(int from, MoveList& legalmoves, const std::array<int, 8>& offsetList) const;

        // appenders
        void addPseudoPawn(int from, MoveList& legalmoves) const;
        void addPseudoKnight(int from, MoveList& legalmoves) const;
        void addPseudoBishop(int from, MoveList& legalmoves) const;
        void addPseudoRook(int from, MoveList& legalmoves) const;
        void addPseudoQueen(int from, MoveList& legalmoves) const;
        void addPseudoKing(int from, MoveList& legalmoves) const;  

        // member variables
        BoardPtr boardPtr_;
};