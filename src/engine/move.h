#pragma once

#include <cstdint>
#include "piece_utils.h"
#include "chess.h"
#include <vector>

class Move {
    public:
        static constexpr unsigned int QUIET_FLAG            = 0b0000;
        static constexpr unsigned int DOUBLE_PAWN_PUSH_FLAG = 0b0001;
        static constexpr unsigned int KING_CASTLE_FLAG      = 0b0010;
        static constexpr unsigned int QUEEN_CASTLE_FLAG     = 0b0011;
        static constexpr unsigned int CAPTURE_FLAG          = 0b0100;
        static constexpr unsigned int EP_CAPTURE_FLAG       = 0b0101;
        static constexpr unsigned int PROMOTION_FLAG        = 0b1000;

        Move() : move_(0), score_(0) {}
        Move(int from, int to, uint8_t pieceType) : score_(0) {
            int flags = QUIET_FLAG;
            move_ = ((pieceType & 0x07)<<18) | ((flags & 0x0F)<<14) | ((from & 0x7F)<<7) | (to & 0x7F);
        }
        Move(int from, int to, int flags, uint8_t pieceType) : score_(0) {
            move_ = ((pieceType & 0x07)<<18) | ((flags & 0x0F)<<14) | ((from & 0x7F)<<7) | (to & 0x7F);
        }

        // getters
        int getTo() const {return move_ & 0x7F;}
        int getFrom() const {return (move_ >> 7) & 0x7F;}
        int getFlags() const {return (move_ >> 14) & 0x0F;}
        int getPieceType() const {return (move_ >> 18) & 0x07;}
        int getCapturePieceType() const {return (move_ >> 21) & 0x07;}

        // flag getters
        bool isFlag(int flag) const {return (getFlags() & FLAG_MASK) == flag;}
        bool isDoublePush() const {return isFlag(DOUBLE_PAWN_PUSH_FLAG);}
        bool isCapture() const {return isFlag(CAPTURE_FLAG);}
        bool isEp() const {return isFlag(EP_CAPTURE_FLAG);}
        bool isKingCastle() const {return isFlag(KING_CASTLE_FLAG);}
        bool isQueenCastle() const {return isFlag(QUEEN_CASTLE_FLAG);}
        bool isCastle() const {return isKingCastle() || isQueenCastle();}

        chess::Tile getToTile() const {return chess::tileOf(getTo());}

        // setters
        void setFlag(int flag) {move_ |= (flag << 14);}
        void setCaptureFlag() {setFlag(CAPTURE_FLAG);}

        void printMove();
    private:
    
    static constexpr unsigned int FLAG_MASK = 0b1111;
    // | capture piece type 3 bits | piece type 3 bits | flags 4 bits | from 7 bits | to 7 bits | (24 bits)
    uint32_t score_;
    uint32_t move_;
};

using MoveList = std::vector<Move>;