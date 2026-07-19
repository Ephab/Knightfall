//
// Created by Firas on 17/07/2026.
//

#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include <optional>
#include "board.h"
#include "types.h"

struct Move{
    Square sourceSquare;
    Square destinationSquare;

    Piece promotionPiece = Piece::EMPTY;
    Piece capturedPiece = Piece::EMPTY;

    bool isCastling = false;
    bool isEnPassant = false;
};

std::vector<Move> generateMoves();
std::vector<Move> generatePawnMoves(Board &board);
std::vector<Move> generateKnightMoves(Board &board);


bool isMoveLegal(Move move);
bool makeMove(Move move);

std::ostream& operator<<(std::ostream& os, const Move& move);

#endif //CHESS_MOVE_H
