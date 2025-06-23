#include <engine/board.h>

namespace {
    constexpr int ctoi(char c) noexcept {return c - '0';}
}

Board::Board(const std::string& fen)
    : fen_m(fen)
{
    validateFen();
    initializeFromFen();
}

bool Board::isEmpty(int file, int rank) const {
    int index = fileRankToIndex(file, rank);
    return piece::getTypeID(boardArr_m[index]) == 0;
}

uint8_t Board::getPieceAt(int file, int rank) const {
    int index = fileRankToIndex(file, rank);
    return boardArr_m[index];
}

void Board::makeMove(int startFile, int startRank, int endFile, int endRank) {
    int startIndex = fileRankToIndex(startFile, startRank);
    int endIndex = fileRankToIndex(endFile, endRank);

    boardArr_m[endIndex] = boardArr_m[startIndex];
    boardArr_m[startIndex] = 0;
}

int Board::fileRankToIndex(int file, int rank) const {
    if (file > 7 || file < 0 || rank > 7 || rank < 0) {
        throw std::invalid_argument("file and rank must be between 0 and 7 inclusive");
    }
    return file + rank*8;
}

void Board::validateFen() {
    //TODO
}

void Board::initializeFromFen() {
    int i = 0;
    for (auto c : fen_m) {
        if (c == '/') {
            continue;
        } else if (c == ' ') {
            break;
        } else if (isdigit(c)) { 
            i+=ctoi(c);
        } else {
            uint8_t piece = piece::charToInt(std::tolower(c)) | (std::islower(c) ? piece::WHITE : piece::BLACK);
            // std::cout << (int)piece << std::endl;
            boardArr_m[i] = piece;
            ++i;
        }
    }

    // for (auto p : boardArr_m) {
    //     std::cout << (int)p << std::endl;
    // }
}