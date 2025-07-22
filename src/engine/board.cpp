#include "engine/board.h"

#include <format>
#include <iostream>
#include <stdexcept>
#include <cctype>

namespace {
    constexpr int ctoi(char c) noexcept {return c - '0';}
}

Board::Board(const std::string& fen)
    : fen_(fen)
{
    validateFen();
    initializeFromFen();
}

bool Board::isEmptyAt(int index) const {
    return piece::getType(boardArr_[index]) == piece::NONE;
}

bool Board::isEmptyAt(chess::Tile tile) const {
    return isEmptyAt(indexOf(tile));
}

uint8_t Board::getPieceAt(int index) const {
    return boardArr_[index];
}

uint8_t Board::getPieceAt(chess::Tile tile) const {
    return getPieceAt(indexOf(tile));
}

void Board::handleEnPassant(Move move) {
    if (move.isEp()) {
        boardArr_[ep_.pawnPos] = piece::NONE;
    } else if (move.isDoublePush()) {
        int offset = isWhiteTurn_ ? -0x10 : 0x10;
        ep_.pawnPos = move.getTo();
        ep_.epPos = ep_.pawnPos + offset;
    } else {
        ep_.pawnPos = -1;
        ep_.epPos = -1;
    }
}

void Board::handleCastle(Move move) {
    if (move.isKingCastle()) {
        int kingSideRookPos = isWhiteTurn_ ? 0x07 : 0x77;
        uint8_t kingSideRook = boardArr_[kingSideRookPos];
        boardArr_[kingSideRookPos] = piece::NONE;
        boardArr_[kingSideRookPos - 2] = kingSideRook | piece::HAS_MOVED;
    } else if (move.isQueenCastle()) {
        int queenSideRookPos = isWhiteTurn_ ? 0x00 : 0x70;
        uint8_t queenSideRook = boardArr_[queenSideRookPos];
        boardArr_[queenSideRookPos] = piece::NONE;
        boardArr_[queenSideRookPos + 3] = queenSideRook | piece::HAS_MOVED;
    }
}



void Board::makeMove(Move move) {
    move.printMove();
    // std::cout << std::dec << (move.getFlags() & move.QUEEN_CASTLE_FLAG) << "\n";
    // std::cout << move.isQueenCastle() << " " << move.isKingCastle() << "\n";
    int src = move.getFrom();
    int dst = move.getTo();

    handleEnPassant(move);
    handleCastle(move);

    if (!isEmptyAt(dst)) {
        if (piece::isWhite(boardArr_[dst])) {
            wVal_ -= piece::valueOf(boardArr_[dst]);
        } else {
            bVal_ -= piece::valueOf(boardArr_[dst]);
        }
    }

    executeMove(src, dst);
}

void Board::executeMove(int from, int to) {
    boardArr_[to] = boardArr_[from] | piece::HAS_MOVED;
    boardArr_[from] = piece::NONE;
    isWhiteTurn_ = !isWhiteTurn_;
}

void Board::validateFen() {
    //TODO
}

void Board::initializeFromFen() {
    std::array<uint8_t, 64> temp = {0};
    int i = 0;
    for (auto c : fen_) {
        if (c == '/') {
            continue;
        } else if (c == ' ') {
            break;
        } else if (isdigit(c)) { 
            i+=ctoi(c);
        } else {
            uint8_t color = std::islower(c) ? piece::WHITE : piece::BLACK;
            uint8_t type = piece::fromChar(std::tolower(c));

            uint8_t piece = type | color;

            temp[i] = piece;

            if (piece::isWhite(piece)) {
                wVal_ += piece::valueOf(piece);
            } else {
                bVal_ += piece::valueOf(piece);
            }

            ++i;
        }
    }

    for (int j = 0; j < 64; ++j) {
        int sq0x88 = j + (j & ~7);
        boardArr_[sq0x88] = temp[j];
    }
}