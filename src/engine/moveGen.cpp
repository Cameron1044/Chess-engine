#include "engine/moveGen.h"
#include <stdexcept>

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

constexpr bool notOnBoard(int from) noexcept {return (from & 0x88);}
constexpr bool onBoard(int from) noexcept {return !notOnBoard(from);}

}

void MoveGen::getPseudoMovesAtTile(chess::Tile tile, MoveList& legalMoves) const {
    legalMoves.clear();
    MoveList tempLegalMoves = getPseudoMovesAt(chess::indexOf(tile));
    for (auto move : tempLegalMoves) {
        legalMoves.push_back(move);
    }
}

MoveList MoveGen::getPseudoMovesAt(int from) const {
    MoveList legalmoves;

    uint8_t piece = boardPtr_->getPieceAt(from);
    if (piece::isWhite(piece) != boardPtr_->getIsWhite()) {
        return legalmoves;
    }

    switch (piece::getType(piece)) {
        case piece::NONE: return {};
        case piece::PAWN: 
            addPseudoPawn(from, legalmoves); 
            break;
        case piece::KNIGHT: 
            addPseudoKnight(from, legalmoves); 
            break;
        case piece::BISHOP:
            addPseudoBishop(from, legalmoves); 
            break;
        case piece::ROOK: 
            addPseudoRook(from, legalmoves); 
            break;
        case piece::QUEEN: 
            addPseudoQueen(from, legalmoves); 
            break;
        case piece::KING: 
            addPseudoKing(from, legalmoves); 
            break;
        default: return {};
    }
    return legalmoves;
}

void MoveGen::addContinuousFromOffsetList(int from, MoveList& legalmoves, const std::array<int, 4>& offsetList) const {
    uint8_t piece = boardPtr_->getPieceAt(from);

    for (auto offset : offsetList) {
        for (int d = 1; onBoard(from + d*offset); ++d) {
            int to = from + d*offset;
            uint8_t targetPiece = boardPtr_->getPieceAt(to);
            Move move{from, to, piece};

            if (piece::sameTeam(piece, targetPiece))
                break;

            if (piece::diffTeam(piece, targetPiece)) {
                move.setCaptureFlag();
                legalmoves.push_back(move);
                break;
            }
            legalmoves.push_back(move);
        }
    }
}

void MoveGen::addFromOffsetList(int from, MoveList& legalmoves, const std::array<int, 8>& offsetList) const {
    uint8_t piece = boardPtr_->getPieceAt(from);

    for (auto offset : offsetList) {
        int to = from + offset;
        uint8_t targetPiece = boardPtr_->getPieceAt(to);
        Move move{from, to, piece::getType(piece)};

        if (notOnBoard(to) || piece::sameTeam(piece, targetPiece)) 
            continue;

        if (piece::diffTeam(piece, targetPiece)) {
            move.setCaptureFlag();
        }
        legalmoves.push_back(move);
    }
}

void MoveGen::addPseudoPawn(int from, MoveList& legalmoves) const {
    const uint8_t pawn = boardPtr_->getPieceAt(from);
    const int dir = piece::isWhite(pawn) ? 1 : -1;
    const int dU = U*dir;
    const int dUU = dU + dU;

    for (auto horzDir : pawnCaptureMoves) {
        const int to = from + horzDir + dU;
        const uint8_t targetPiece = boardPtr_->getPieceAt(to);

        if (notOnBoard(to) || piece::sameTeam(pawn, targetPiece)) 
            continue;

        Move captureMove{from, to, pawn};

        if (piece::diffTeam(pawn, targetPiece)) {
            captureMove.setCaptureFlag();
            legalmoves.push_back(captureMove);
        } else if (to == boardPtr_->getEp()) {
            captureMove.setFlag(captureMove.EP_CAPTURE_FLAG);
            legalmoves.push_back(captureMove);
        }
    }

    if (boardPtr_->isEmptyAt(from + dU)) {
        Move singleMove{from, from + dU, pawn};
        legalmoves.push_back(singleMove);

        if (!piece::hasMoved(pawn) && boardPtr_->isEmptyAt(from + dUU)) {
            Move doubleMove{from, from + dUU, pawn};
            doubleMove.setFlag(doubleMove.DOUBLE_PAWN_PUSH_FLAG);
            legalmoves.push_back(doubleMove);
        }
    }
}

void MoveGen::addPseudoKnight(int from, MoveList& legalmoves) const {
    addFromOffsetList(from, legalmoves, knightMoves);
}

void MoveGen::addPseudoBishop(int from, MoveList& legalmoves) const {
    addContinuousFromOffsetList(from, legalmoves, diagonalMoves);
}

void MoveGen::addPseudoRook(int from, MoveList& legalmoves) const {
    addContinuousFromOffsetList(from, legalmoves, lateralMoves);
}

void MoveGen::addPseudoQueen(int from, MoveList& legalmoves) const {
    addContinuousFromOffsetList(from, legalmoves, diagonalMoves);
    addContinuousFromOffsetList(from, legalmoves, lateralMoves);
}

void MoveGen::addPseudoKing(int from, MoveList& legalmoves) const {
    addFromOffsetList(from, legalmoves, kingMoves);

    const uint8_t king = boardPtr_->getPieceAt(from);
    if (piece::hasMoved(king)) // king has moved, don't check for castle
        return;

    int kingSideRookPos = piece::isWhite(king) ? 0x07 : 0x77;
    uint8_t kingSideRook = boardPtr_->getPieceAt(kingSideRookPos);
    int queenSideRookPos = piece::isWhite(king) ? 0x00 : 0x70;
    uint8_t queenSideRook = boardPtr_->getPieceAt(queenSideRookPos);
    bool isEmptyBetween = false;

    // kingside castle
    isEmptyBetween = boardPtr_->isEmptyAt(from + 1) && boardPtr_->isEmptyAt(from + 2);
    if (!piece::hasMoved(kingSideRook) && isEmptyBetween) {
        int to = from + 2;
        Move move{from, to, king};
        move.setFlag(move.KING_CASTLE_FLAG);
        legalmoves.push_back(move);
    }

    // queenside castle
    isEmptyBetween = boardPtr_->isEmptyAt(from - 1) && boardPtr_->isEmptyAt(from - 2) && boardPtr_->isEmptyAt(from - 3);
    if (!piece::hasMoved(queenSideRook) && isEmptyBetween) {
        int to = from - 2;
        Move move{from, to, king};
        move.setFlag(move.QUEEN_CASTLE_FLAG);
        legalmoves.push_back(move);
    }
}