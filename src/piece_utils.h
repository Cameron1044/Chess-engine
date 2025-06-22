#pragma once

#include <cstdint>

namespace piece {
    constexpr inline uint8_t TYPE_MASK = 0b0000'0111;
    constexpr inline uint8_t COLOR_MASK = 0b0000'1000;

    constexpr inline uint8_t NONE = 0;
    constexpr inline uint8_t PAWN = 1;
    constexpr inline uint8_t BISHOP = 2;
    constexpr inline uint8_t KNIGHT = 3;
    constexpr inline uint8_t ROOK = 4;
    constexpr inline uint8_t KING = 5;
    constexpr inline uint8_t QUEEN = 6;
    constexpr inline uint8_t GHOST_PAWN = 7;

    constexpr inline uint8_t WHITE = 0;
    constexpr inline uint8_t BLACK = COLOR_MASK;

    constexpr inline uint8_t getTypeID(uint8_t p) noexcept {return p & TYPE_MASK;}
    constexpr inline bool isWhite(uint8_t p) noexcept {return (p & COLOR_MASK) == WHITE;}
    constexpr inline char intToChar(uint8_t p) {
        switch (p) {
            case PAWN: return 'p';
            case BISHOP: return 'b';
            case KNIGHT: return 'n';
            case ROOK: return 'r';
            case KING: return 'k';
            case QUEEN: return 'q';
            default: return ' ';
        }
    }
    constexpr inline uint8_t charToInt(char p) {
        switch (p) {
            case 'p': return PAWN;
            case 'b': return BISHOP;
            case 'n': return KNIGHT;
            case 'r': return ROOK;
            case 'k': return KING;
            case 'q': return QUEEN;
            default: return NONE;
        }
    }
}