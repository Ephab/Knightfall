//
// Created by Firas on 16/07/2026.
//

#include "board.h"

#include <iostream>
#include <optional>
#include <ostream>

#include "types.h"

Board::Board(){
    sideToMove = Color::WHITE;

    for (int i = 0; i < 64; i++){
        pieceMap[i] = Piece::EMPTY;
    }

    whitePawns = 0;
    whiteKnights = 0;
    whiteBishops = 0;
    whiteKing = 0;
    whiteRooks = 0;
    whiteQueens = 0;

    blackPawns = 0;
    blackKnights = 0;
    blackBishops = 0;
    blackKing = 0;
    blackRooks = 0;
    blackQueens = 0;
}

void Board::initializeStartingPosition(){
    whitePawns = 0x000000000000FF00ULL;
    whiteKnights = 0x0000000000000042ULL;
    whiteBishops = 0x0000000000000024ULL;
    whiteKing = 0x0000000000000010ULL;
    whiteRooks = 0x0000000000000081ULL;
    whiteQueens = 0x0000000000000008ULL;

    blackPawns = 0x00FF000000000000ULL;
    blackKnights = 0x4200000000000000ULL;
    blackBishops = 0x2400000000000000ULL;
    blackKing = 0x1000000000000000ULL;
    blackRooks = 0x8100000000000000ULL;
    blackQueens = 0x0800000000000000ULL;

    // PAWNS
    const int rank2 = 8;
    const int rank7 = 48;
    for (int i = 0; i < 8; ++i){
        setPiece(static_cast<Square>(rank2 + i), Piece::WHITE_PAWN);
        setPiece(static_cast<Square>(rank7 + i), Piece::BLACK_PAWN);
    }
    // KNIGHTS
    setPiece(Square::B1, Piece::WHITE_KNIGHT);
    setPiece(Square::G1, Piece::WHITE_KNIGHT);
    setPiece(Square::B8, Piece::BLACK_KNIGHT);
    setPiece(Square::G8, Piece::BLACK_KNIGHT);
    // BISHOPS
    setPiece(Square::C1, Piece::WHITE_BISHOP);
    setPiece(Square::F1, Piece::WHITE_BISHOP);
    setPiece(Square::C8, Piece::BLACK_BISHOP);
    setPiece(Square::F8, Piece::BLACK_BISHOP);
    // ROOKS
    setPiece(Square::A1, Piece::WHITE_ROOK);
    setPiece(Square::H1, Piece::WHITE_ROOK);
    setPiece(Square::A8, Piece::BLACK_ROOK);
    setPiece(Square::H8, Piece::BLACK_ROOK);
    // QUEENS
    setPiece(Square::D1, Piece::WHITE_QUEEN);
    setPiece(Square::D8, Piece::BLACK_QUEEN);
    // KINGS
    setPiece(Square::E1, Piece::WHITE_KING);
    setPiece(Square::E8, Piece::BLACK_KING);

}

void Board::setBit(U64& bitboard, Square position){
    U64 mask = 1ULL << static_cast<int>(position);
    bitboard |= mask;
}

void Board::clearBit(U64& bitboard, Square position){
    U64 mask = 1ULL << static_cast<int>(position);
    bitboard &= ~mask;
}

bool Board::isBitSet(U64& bitboard, Square position){
    U64 mask = 1ULL << static_cast<int>(position);
    return (bitboard & mask) != 0;
}

void Board::setPiece(Square position, Piece piece){
    pieceMap[static_cast<int>(position)] = piece;
}

Piece Board::getPiece(Square position) const{
    return pieceMap[static_cast<int>(position)];
}

bool Board::isEmpty(Square position) const{
    return getPiece(position) == Piece::EMPTY;
}

bool Board::isOccupied(Square position) const{
    return !isEmpty(position);
}

void Board::clearPiece(Square position){
    pieceMap[static_cast<int>(position)] = Piece::EMPTY;
}

std::optional<Color> Board::getPieceColor(Piece piece){
    if (piece == Piece::EMPTY){
        return std::nullopt;
    }
    if (piece == Piece::WHITE_PAWN ||
        piece == Piece::WHITE_KNIGHT ||
        piece == Piece::WHITE_BISHOP ||
        piece == Piece::WHITE_ROOK ||
        piece == Piece::WHITE_QUEEN ||
        piece == Piece::WHITE_KING){
        return Color::WHITE;
    }
    return Color::BLACK;
}

void Board::printBoard(){
    for (int rank = 7; rank >= 0; --rank){
        std::cout << rank + 1 << " ";

        for (int file = 0; file < 8; ++file){
            int index = rank * 8 + file;
            std::cout << printPiece(pieceMap[index]) << " ";
        }

        std::cout << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl;
}

char Board::printPiece(Piece enumObject) {
    switch (enumObject) {
        case Piece::WHITE_PAWN:
            return 'P';
        case Piece::WHITE_KNIGHT:
            return 'N';
        case Piece::WHITE_BISHOP:
            return 'B';
        case Piece::WHITE_ROOK:
            return 'R';
        case Piece::WHITE_QUEEN:
            return 'Q';
        case Piece::WHITE_KING:
            return 'K';
        case Piece::BLACK_PAWN:
            return 'p';
        case Piece::BLACK_KNIGHT:
            return 'n';
        case Piece::BLACK_BISHOP:
            return 'b';
        case Piece::BLACK_ROOK:
            return 'r';
        case Piece::BLACK_QUEEN:
            return 'q';
        case Piece::BLACK_KING:
            return 'k';
        case Piece::EMPTY:
            return '.';

        default:
            throw std::invalid_argument("Invalid piece");
    }
}
