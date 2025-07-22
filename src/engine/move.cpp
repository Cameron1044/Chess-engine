#include "engine/move.h"
#include <iostream>
#include <format>
#include <bitset>

void Move::printMove() {
    int to = getTo();
    int from = getFrom();
    std::bitset<4> flags = getFlags();
    std::bitset<3> pieceType = getPieceType();
    std::bitset<3> capturePieceType = getCapturePieceType();

    std::cout << capturePieceType << " " << pieceType << " " << flags << " " << std::hex << from << " " << to << "\n";
}