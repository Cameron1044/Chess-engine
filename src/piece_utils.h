#pragma once

#include <cstdint>

namespace piece {

constexpr inline uint8_t TYPE_MASK = 0b0000'0111;
constexpr inline uint8_t TEAM_MASK = 0b0000'1000;
constexpr inline uint8_t HAS_MOVED_MASK = 0b0001'0000;

constexpr inline uint8_t NONE = 0;
constexpr inline uint8_t PAWN = 1;
constexpr inline uint8_t BISHOP = 2;
constexpr inline uint8_t KNIGHT = 3;
constexpr inline uint8_t ROOK = 4;
constexpr inline uint8_t KING = 5;
constexpr inline uint8_t QUEEN = 6;
constexpr inline uint8_t GHOST_PAWN = 7;

constexpr inline uint8_t WHITE = 0;
constexpr inline uint8_t BLACK = TEAM_MASK;

constexpr inline uint8_t HAS_MOVED = HAS_MOVED_MASK;

constexpr inline uint8_t getType(uint8_t p) noexcept {return p & TYPE_MASK;}
constexpr inline bool isWhite(uint8_t p) noexcept {return (p & TEAM_MASK) == WHITE;}

constexpr inline bool sameTeam(uint8_t p1, uint8_t p2) noexcept {
    return (p2 != NONE) && ((p1 & TEAM_MASK) == (p2 & TEAM_MASK));
}

// constexpr inline bool canMove(uint8_t p1, uint8_t p2) noexcept {
//     return p2 != NONE && (p1 & TEAM_MASK) == (p2 & TEAM_MASK);
// }

constexpr inline bool hasMoved(uint8_t p) noexcept {return (p & HAS_MOVED_MASK) == HAS_MOVED;}

inline char toChar(uint8_t p) {
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

inline uint8_t fromChar(char p) {
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