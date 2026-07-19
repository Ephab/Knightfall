//
// Created by Firas on 16/07/2026.
//

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <optional>
#include "types.h"
#include <string>
#include <array>
#include <vector>


class Board{
public:
    inline static constexpr std::array<Square, 64> positions = {
        Square::A1, Square::B1, Square::C1, Square::D1, Square::E1, Square::F1, Square::G1, Square::H1,
        Square::A2, Square::B2, Square::C2, Square::D2, Square::E2, Square::F2, Square::G2, Square::H2,
        Square::A3, Square::B3, Square::C3, Square::D3, Square::E3, Square::F3, Square::G3, Square::H3,
        Square::A4, Square::B4, Square::C4, Square::D4, Square::E4, Square::F4, Square::G4, Square::H4,
        Square::A5, Square::B5, Square::C5, Square::D5, Square::E5, Square::F5, Square::G5, Square::H5,
        Square::A6, Square::B6, Square::C6, Square::D6, Square::E6, Square::F6, Square::G6, Square::H6,
        Square::A7, Square::B7, Square::C7, Square::D7, Square::E7, Square::F7, Square::G7, Square::H7,
        Square::A8, Square::B8, Square::C8, Square::D8, Square::E8, Square::F8, Square::G8, Square::H8
    };

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
    void clearPiece(Square position);

    static std::optional<Color> getPieceColor(Piece piece);

    bool isEmpty(Square position) const;
    bool isOccupied(Square position) const;

    static File getFile(Square position);
    static Rank getRank(Square position);

    static char printPiece(Piece enumObject);
    static std::string printSquare(Square square);
    void printBoard() const;

    static std::optional<Square> getSquareOffset(Square square, int offsetX, int offsetY);
};


#endif //CHESS_BOARD_H
