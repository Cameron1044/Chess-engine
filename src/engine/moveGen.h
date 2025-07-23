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

        bool isMoveLegal(const Move& move);
        bool isKingChecked() const;

        MoveList generateLegalMoves() {return generateLegalMoves(boardPtr_->getColorToPlay());};
        MoveList generateLegalMoves(bool color);

        MoveList generateLegalMovesOf(chess::Tile tile) {return generateLegalMovesOf(chess::indexOf(tile));};
        MoveList generateLegalMovesOf(int from);

        MoveList generatePseudoMoves() const {return generatePseudoMoves(boardPtr_->getColorToPlay());};
        MoveList generatePseudoMoves(bool color) const;

        // getters
        // MoveList getPseudoPawn(int from, MoveList& legalmoves) const;
        // MoveList getPseudoKnight(int from, MoveList& legalmoves) const;
        // MoveList getPseudoBishop(int from, MoveList& legalmoves) const;
        // MoveList getPseudoRook(int from, MoveList& legalmoves) const;
        // MoveList getPseudoQueen(int from, MoveList& legalmoves) const;
        // MoveList getPseudoKing(int from, MoveList& legalmoves) const;
        static constexpr uint8_t MAX_LEGAL_MOVES = 218;

    private:
        // helpers
        static void swapAndPop(MoveList& moves, uint8_t index);

        // appender helpers
        void addContinuousFromOffsetList(int from, MoveList& legalmoves, const std::array<int, 4>& offsetList) const;
        void addFromOffsetList(int from, MoveList& legalmoves, const std::array<int, 8>& offsetList) const;

        // appenders
        void addPseudoMovesAt(int from, MoveList& legalMoves, bool color) const;
        void addPseudoPawn(int from, MoveList& legalmoves) const;
        void addPseudoKnight(int from, MoveList& legalmoves) const;
        void addPseudoBishop(int from, MoveList& legalmoves) const;
        void addPseudoRook(int from, MoveList& legalmoves) const;
        void addPseudoQueen(int from, MoveList& legalmoves) const;
        void addPseudoKing(int from, MoveList& legalmoves) const;  

        // member variables
        BoardPtr boardPtr_;
};