#include "engine/board.h"

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

void Board::makeMove(chess::Tile start, chess::Tile end) {
    int startIndex = chess::indexOf(start);
    int endIndex = chess::indexOf(end);

    boardArr_[endIndex] = boardArr_[startIndex] | piece::HAS_MOVED;
    boardArr_[startIndex] = piece::NONE;
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
            uint8_t piece = piece::fromChar(std::tolower(c)) | (std::islower(c) ? piece::WHITE : piece::BLACK);
            temp[i] = piece;
            ++i;
        }
    }

    for (int j = 0; j < 64; ++j) {
        int sq0x88 = j + (j & ~7);
        boardArr_[sq0x88] = temp[j];
    }
}