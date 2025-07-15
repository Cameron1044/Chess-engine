#pragma once

#include <cstdlib>

namespace chess {

inline constexpr int tileSize = 100;

struct Coord {
    int x;
    int y;

    // dereference comparison overload
    bool operator==(const Coord& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
};

struct Tile {
    int file;
    int rank;

    // dereference comparison overload
    bool operator==(const Tile& rhs) const {
        return file == rhs.file && rank == rhs.rank;
    }
};

// index convertions
inline int fileOf(int index) noexcept {return index & 7;} // index % 8
inline int rankOf(int index) noexcept {return index >> 4;} // index / 16
inline Tile tileOf(int index) noexcept {return Tile{fileOf(index), rankOf(index)};}

// tile convertions
inline int indexOf(Tile tile) noexcept {return tile.file + tile.rank*16;}

// coord convertions
inline Tile tileOf(Coord coord) noexcept {return Tile{coord.x/100, abs(coord.y/100 - 7)};}
}