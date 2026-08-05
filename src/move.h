//
// Created by Firas on 17/07/2026.
//

#pragma once

#include <optional>
#include <vector>
#include "types.h"

class Board;

struct Move{
    Square sourceSquare;
    Square destinationSquare;

    Piece promotionPiece = Piece::EMPTY;
    Piece capturedPiece = Piece::EMPTY;

    bool isCastling = false;
    bool isEnPassant = false;
};

struct UndoInfo{
    Move move;

    Piece startingPiece;
    Color sideToMove;

    bool whiteCanCastleKingside = true;
    bool whiteCanCastleQueenside = true;

    bool blackCanCastleKingside = true;
    bool blackCanCastleQueenside = true;

    std::optional<Square> enPassentSquare = std::nullopt;
};

std::vector<Move> generateMoves();
std::vector<Move> generatePawnMoves(Board& board, Square pawnSquare, bool white);

std::vector<Move> generateKnightMoves(Board& board, Square knightSquare, bool white);
std::vector<Square> getPossibleKnightDestinations(Square square);

std::vector<Move> generateBishopMoves(Board& board, Square bishopSquare, bool white);
std::vector<Move> getDiagonalMoves(Board& board, Square square);

std::vector<Move> generateRookMoves(Board& board, Square rookSquare, bool white);
std::vector<Move> getStraightMoves(Board& board, Square square);

std::vector<Move> generateQueenMoves(Board& board, Square queenSquare, bool white);

std::vector<Move> generateKingMoves(Board& board, Square kingSquare, bool white);
std::vector<Square> getPossibleKingDestinations(Square square);

std::vector<Move> generateAllMoves(Board& board);

bool isMoveLegal(Move move);
bool makeMove(Move move);

std::ostream& operator<<(std::ostream& os, const Move& move);
