//
// Created by Firas on 17/07/2026.
//

#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H
#include <optional>

#include "types.h"

class Move{
    Square sourceSquare;
    Square destinationSquare;
    Piece pieceType;
    bool isCapture;
    Piece promotionPiece;
    bool isCastling;
    bool isEnPassant;
};

bool isMoveLegal(Move move);
bool makeMove(Move move);

#endif //CHESS_MOVE_H
