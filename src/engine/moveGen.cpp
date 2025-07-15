#include "engine/moveGen.h"

/*
0x88 Board Representation: 

70 71 72 73 74 75 76 77 | 78 79 7A 7B 7C 7D 7E 7F
60 61 62 63 64 65 66 67 | 68 69 6A 6B 6C 6D 6E 6F
50 51 52 53 54 55 56 57 | 58 59 5A 5B 5C 5D 5E 5F
40 41 42 43 44 45 46 47 | 48 49 4A 4B 4C 4D 4E 4F
30 31 32 33 34 35 36 37 | 38 39 3A 3B 3C 3D 3E 3F
20 21 22 23 24 25 26 27 | 28 29 2A 2B 2C 2D 2E 2F
10 11 12 13 14 15 16 17 | 18 19 1A 1B 1C 1D 1E 1F
00 01 02 03 04 05 06 07 | 08 09 0A 0B 0C 0D 0E 0F
*/

namespace {

constexpr int U = 0x10;
constexpr int D = -0x10;
constexpr int R = 0x1;
constexpr int L = -0x1;
constexpr int UR = U + R;
constexpr int UL = U + L;
constexpr int DR = D + R;
constexpr int DL = D + L;

constexpr std::array<int, 8> knightMoves = {U+UR, U+UL, 
                                            D+DR, D+DL,
                                            UR+R, DR+R, 
                                            UL+L, DL+L};
constexpr std::array<int, 8> kingMoves = {U, D, L, R, UR, UL, DR, DL};
constexpr std::array<int, 4> lateralMoves = {U, D, L, R};
constexpr std::array<int, 4> diagonalMoves = {UR, UL, DR, DL};
constexpr std::array<int, 2> pawnCaptureMoves = {L, R};

constexpr bool notOnBoard(int index) noexcept {return (index & 0x88);}
constexpr bool onBoard(int index) noexcept {return !notOnBoard(index);}

}

void MoveGen::genLegalMovesAtTile(chess::Tile tile, const Board& board, std::vector<chess::Tile>& legalMoves) {
    legalMoves.clear();
    std::vector<int> legalMovesInt = genLegalMovesAt(chess::indexOf(tile), board);
    for (auto moveIndex : legalMovesInt) {
        legalMoves.push_back(chess::tileOf(moveIndex));
    }
}

std::vector<int> MoveGen::genLegalMovesAt(int index, const Board& board) {
    std::vector<int> legalmoves;

    switch (piece::getType(board.getPieceAt(index))) {
        case piece::NONE: return {};
        case piece::PAWN: 
            genPawn(index, legalmoves, board); 
            break;
        case piece::KNIGHT: 
            genKnight(index, legalmoves, board); 
            break;
        case piece::BISHOP:
            genBishop(index, legalmoves, board); 
            break;
        case piece::ROOK: 
            genRook(index, legalmoves, board); 
            break;
        case piece::QUEEN: 
            genQueen(index, legalmoves, board); 
            break;
        case piece::KING: 
            genKing(index, legalmoves, board); 
            break;
        default: return {};
    }
    return legalmoves;
}

void MoveGen::genLateral(int index, std::vector<int>& legalmoves, const Board& board) {
    uint8_t piece = board.getPieceAt(index);

    for (auto move : lateralMoves) {
        for (int d = 1; onBoard(index + d*move); ++d) {
            int targetIndex = index + d*move;
            uint8_t targetPiece = board.getPieceAt(targetIndex);

            if (!piece::sameTeam(piece, targetPiece)) {
                legalmoves.push_back(targetIndex);
            }

            if (!board.isEmptyAt(targetIndex)) 
                break;
        }
    }
}

void MoveGen::genDiagonal(int index, std::vector<int>& legalmoves, const Board& board) {
    uint8_t piece = board.getPieceAt(index);

    for (auto move : diagonalMoves) {
        for (int d = 1; onBoard(index + d*move); ++d) {
            int targetIndex = index + d*move;
            uint8_t targetPiece = board.getPieceAt(targetIndex);

            if (!piece::sameTeam(piece, targetPiece)) {
                legalmoves.push_back(targetIndex);
            }

            if (!board.isEmptyAt(targetIndex)) 
                break;
        }
    }
}

void MoveGen::genPawn(int index, std::vector<int>& legalmoves, const Board& board) {
    uint8_t pawn = board.getPieceAt(index);
    int dir = piece::isWhite(pawn) ? 1 : -1;
    int dU = U*dir;
    int dUU = dU + dU;

    for (auto horzDir : pawnCaptureMoves) {
        int captureIndex = index + dU + horzDir;

        if (notOnBoard(captureIndex)) 
            continue;

        uint8_t targetPiece = board.getPieceAt(captureIndex);
        if (!board.isEmptyAt(captureIndex) && !piece::sameTeam(pawn, targetPiece)) {
            legalmoves.push_back(captureIndex);
        }
    }

    if (board.isEmptyAt(index + dU)) {
        legalmoves.push_back(index + dU);

        if (!piece::hasMoved(pawn) && board.isEmptyAt(index + dUU)) {
            legalmoves.push_back(index + dUU);
        }
    }
}

void MoveGen::genKnight(int index, std::vector<int>& legalmoves, const Board& board) {
    uint8_t knight = board.getPieceAt(index);

    for (auto move : knightMoves) {
        if (notOnBoard(index + move)) 
            continue;

        int targetIndex = index + move;
        uint8_t targetPiece = board.getPieceAt(targetIndex);

        if (!piece::sameTeam(knight, targetPiece)) {
            legalmoves.push_back(targetIndex);
        }
    }
}

void MoveGen::genBishop(int index, std::vector<int>& legalmoves, const Board& board) {
    genDiagonal(index, legalmoves, board);
}

void MoveGen::genRook(int index, std::vector<int>& legalmoves, const Board& board) {
    genLateral(index, legalmoves, board);
}

void MoveGen::genQueen(int index, std::vector<int>& legalmoves, const Board& board) {
    genLateral(index, legalmoves, board);
    genDiagonal(index, legalmoves, board);
}

void MoveGen::genKing(int index, std::vector<int>& legalmoves, const Board& board) {
    uint8_t king = board.getPieceAt(index);

    for (auto move : kingMoves) {
        if (notOnBoard(index + move)) continue;

        int targetIndex = index + move;
        uint8_t targetPiece = board.getPieceAt(targetIndex);

        if (!piece::sameTeam(king, targetPiece)) {
            legalmoves.push_back(targetIndex);
        }
    }
}