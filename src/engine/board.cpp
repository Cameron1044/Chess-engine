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

uint8_t Board::getPieceAt(int index) const {
    return boardArr_m[index];
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