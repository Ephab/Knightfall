//
// Created by Firas on 16/07/2026.
//

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <optional>

#include "types.h"
#include <string>

class Board{
public:
    U64 whitePawns;
    U64 whiteKnights;
    U64 whiteBishops;
    U64 whiteKing;
    U64 whiteRooks;
    U64 whiteQueens;

    U64 blackPawns;
    U64 blackKnights;
    U64 blackBishops;
    U64 blackKing;
    U64 blackRooks;
    U64 blackQueens;

    Color sideToMove;

    Piece pieceMap[64];

    Board();

    void initializeStartingPosition();

    static void setBit(U64& bitboard, Square position);
    static void clearBit(U64& bitboard, Square position);
    static bool isBitSet(U64& bitboard, Square position);

    void setPiece(Square position, Piece piece);
    Piece getPiece(Square position) const;
    bool isEmpty(Square position) const;
    bool isOccupied(Square position) const;
    void clearPiece(Square position);
    static std::optional<Color> getPieceColor(Piece piece);


    static char printPiece(Piece enumObject);
    void printBoard() const;
};


#endif //CHESS_BOARD_H
