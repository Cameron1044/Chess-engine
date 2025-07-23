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

void MoveGen::swapAndPop(MoveList& moves, uint8_t index) {
    if (index > moves.size() - 1) {
        throw std::runtime_error(std::format("index our of range error: SwapAndPop recieved index of {} for a vector of size {}", index, moves.size()));
    }

    if (moves.size() > 1) {
        std::iter_swap(moves.begin() + index, moves.end() - 1);
        moves.pop_back();
    } else {
        moves.clear();
    }
}

bool MoveGen::isMoveLegal(const Move& move) {
    bool isMoveLegal = true;
    boardPtr_->makeMove(move);

    if (isKingChecked()) {
        isMoveLegal = false;
    }

    boardPtr_->unmakeMove(move);
    return isMoveLegal;
}


bool MoveGen::isKingChecked() const {
    const bool color = boardPtr_->getColorToPlay();
    MoveList moveList = generatePseudoMoves(color);

    for (auto move : moveList) {
        if (move.getCapturePieceType() == piece::KING) {
            return true;
        }
    }
    return false;
}

MoveList MoveGen::generateLegalMoves(bool color) {

    MoveList moves = generatePseudoMoves(color);

    if (moves.empty()) {
        return moves;
    }

    for (uint8_t i = 0; i < MAX_LEGAL_MOVES; ++i) {
        Move move = moves[i];

        if (!isMoveLegal(move)) {
            swapAndPop(moves, i);
        }

        if (i >= moves.size()) {
            break;
        }
    }

	return moves;
}

MoveList MoveGen::generateLegalMovesOf(int from) {
    MoveList moves;
    const bool color = boardPtr_->getColorToPlay();
    addPseudoMovesAt(from, moves, color);
    
    for (uint8_t i = 0; i < MAX_LEGAL_MOVES; ++i) {
        if (i >= moves.size())
            break;

        Move move = moves[i];

        if (!isMoveLegal(move)) {
            swapAndPop(moves, i);
        }
    }

	return moves;
}

MoveList MoveGen::generatePseudoMoves(bool color) const {
    MoveList pseudoMoves;
    pseudoMoves.reserve(MAX_LEGAL_MOVES);

    for (int i = 0; i < 64; ++i) {
        int sq0x88 = i + (i & ~7);
        addPseudoMovesAt(i, pseudoMoves, color);
    }

    return pseudoMoves;
}

void MoveGen::addPseudoMovesAt(int from, MoveList& legalMoves, bool color) const {
    const uint8_t piece = boardPtr_->getPieceAt(from);
    if (piece::isWhite(piece) != color) {
        return;
    }

    switch (piece::getType(piece)) {
        case piece::NONE: return;
        case piece::PAWN: 
            addPseudoPawn(from, legalMoves); 
            break;
        case piece::KNIGHT: 
            addPseudoKnight(from, legalMoves); 
            break;
        case piece::BISHOP:
            addPseudoBishop(from, legalMoves); 
            break;
        case piece::ROOK: 
            addPseudoRook(from, legalMoves); 
            break;
        case piece::QUEEN: 
            addPseudoQueen(from, legalMoves); 
            break;
        case piece::KING: 
            addPseudoKing(from, legalMoves); 
            break;
        default: return;
    }
}

void MoveGen::addContinuousFromOffsetList(int from, MoveList& legalMoves, const std::array<int, 4>& offsetList) const {
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
                move.setCapturePieceType(targetPiece);
                legalMoves.push_back(move);
                break;
            }
            legalMoves.push_back(move);
        }
    }
}

void MoveGen::addFromOffsetList(int from, MoveList& legalMoves, const std::array<int, 8>& offsetList) const {
    uint8_t piece = boardPtr_->getPieceAt(from);

    for (auto offset : offsetList) {
        int to = from + offset;
        uint8_t targetPiece = boardPtr_->getPieceAt(to);
        Move move{from, to, piece};

        if (notOnBoard(to) || piece::sameTeam(piece, targetPiece)) 
            continue;

        if (piece::diffTeam(piece, targetPiece)) {
            move.setCaptureFlag();
            move.setCapturePieceType(targetPiece);
        }
        legalMoves.push_back(move);
    }
}

void MoveGen::addPseudoPawn(int from, MoveList& legalMoves) const {
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
            captureMove.setCapturePieceType(targetPiece);
            legalMoves.push_back(captureMove);
        } else if (boardPtr_->canEp() && (to == boardPtr_->getEp())) {
            captureMove.setFlag(captureMove.EP_CAPTURE_FLAG);
            legalMoves.push_back(captureMove);
        }
    }

    if (boardPtr_->isEmptyAt(from + dU)) {
        Move singleMove{from, from + dU, pawn};
        legalMoves.push_back(singleMove);

        if (!piece::hasMoved(pawn) && boardPtr_->isEmptyAt(from + dUU)) {
            Move doubleMove{from, from + dUU, pawn};
            doubleMove.setFlag(doubleMove.DOUBLE_PAWN_PUSH_FLAG);
            legalMoves.push_back(doubleMove);
        }
    }
}

void MoveGen::addPseudoKnight(int from, MoveList& legalMoves) const {
    addFromOffsetList(from, legalMoves, knightMoves);
}

void MoveGen::addPseudoBishop(int from, MoveList& legalMoves) const {
    addContinuousFromOffsetList(from, legalMoves, diagonalMoves);
}

void MoveGen::addPseudoRook(int from, MoveList& legalMoves) const {
    addContinuousFromOffsetList(from, legalMoves, lateralMoves);
}

void MoveGen::addPseudoQueen(int from, MoveList& legalMoves) const {
    addContinuousFromOffsetList(from, legalMoves, diagonalMoves);
    addContinuousFromOffsetList(from, legalMoves, lateralMoves);
}

void MoveGen::addPseudoKing(int from, MoveList& legalMoves) const {
    addFromOffsetList(from, legalMoves, kingMoves);

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
        legalMoves.push_back(move);
    }

    // queenside castle
    isEmptyBetween = boardPtr_->isEmptyAt(from - 1) && boardPtr_->isEmptyAt(from - 2) && boardPtr_->isEmptyAt(from - 3);
    if (!piece::hasMoved(queenSideRook) && isEmptyBetween) {
        int to = from - 2;
        Move move{from, to, king};
        move.setFlag(move.QUEEN_CASTLE_FLAG);
        legalMoves.push_back(move);
    }
}