//
// Created by Firas on 16/07/2026.
//

#include "board.h"

#include <iostream>
#include <optional>
#include <ostream>
#include <array>

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

void Board::setCustomStartingPosition(){
    placePiece(Square::D4, Piece::WHITE_BISHOP);
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

void Board::placePiece(Square position, Piece piece){
    if (piece == Piece::EMPTY) return;

    if (getPiece(position) != Piece::EMPTY)
        throw std::invalid_argument("Square is already occupied at " + printSquare(position));

    U64* bitboard = getBitboardPointer(piece);
    setPiece(position, piece);
    setBit(*bitboard, position);
}

void Board::removePiece(Square position){
    Piece piece = getPiece(position);
    if (piece == Piece::EMPTY) return;

    U64* bitboard = getBitboardPointer(piece);
    clearPiece(position);
    clearBit(*bitboard, position);
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

File Board::getFile(Square position){
    int file = static_cast<int>(position) % 8;
    return static_cast<File>(file);
}

Rank Board::getRank(Square position){
    int rank = static_cast<int>(position) / 8;
    return static_cast<Rank>(rank);
}

void Board::clearPiece(Square position){
    pieceMap[static_cast<int>(position)] = Piece::EMPTY;
}

void Board::makeMove(const Move& move){
    Square sourceSquare = move.sourceSquare;
    Square destinationSquare = move.destinationSquare;

    const bool isCapture = move.capturedPiece != Piece::EMPTY;
    const bool isPromotion = move.promotionPiece != Piece::EMPTY;

    Piece startingPiece = getPiece(sourceSquare);
    Piece destinationPiece = getPiece(sourceSquare);
    Piece capturedPiece = move.capturedPiece;

    //todo: implement this later
    std::optional<Square> enPassantSquare = std::nullopt;

    if (isCapture){
        removePiece(destinationSquare); //remove enemy
    }
    if (isPromotion){
        destinationPiece = move.promotionPiece;
    }

    undoStack.push_back(UndoInfo{
        .move = move,
        .startingPiece = startingPiece,
        .sideToMove = sideToMove,
        .whiteCanCastleKingside = whiteCanCastleKingside,
        .whiteCanCastleQueenside = whiteCanCastleQueenside,
        .blackCanCastleKingside = blackCanCastleKingside,
        .blackCanCastleQueenside = blackCanCastleQueenside,
        .enPassentSquare = enPassantSquare
    });

    removePiece(sourceSquare);
    placePiece(destinationSquare, destinationPiece);

    if (sideToMove == Color::WHITE)
        sideToMove = Color::BLACK;
    else
        sideToMove = Color::WHITE;

    moveHistory.push_back(move);

    //KING MOVED
    if (startingPiece == Piece::WHITE_KING){
        this->whiteCanCastleKingside = false;
        this->whiteCanCastleQueenside = false;
    }
    if (startingPiece == Piece::BLACK_KING){
        this->blackCanCastleKingside = false;
        this->blackCanCastleQueenside = false;
    }
    //ROOKS MOVED
    if (startingPiece == Piece::WHITE_ROOK &&
        sourceSquare == Square::A1)
        this->whiteCanCastleQueenside = false;
    if (startingPiece == Piece::WHITE_ROOK &&
        sourceSquare == Square::H1)
        this->whiteCanCastleKingside = false;
    if (startingPiece == Piece::BLACK_ROOK &&
        sourceSquare == Square::A8)
        this->blackCanCastleQueenside = false;
    if (startingPiece == Piece::BLACK_ROOK &&
        sourceSquare == Square::H8)
        this->blackCanCastleKingside = false;
    //ROOKS CAPTURED
    if (capturedPiece == Piece::WHITE_ROOK &&
        destinationSquare == Square::A1)
        this->whiteCanCastleQueenside = false;
    if (capturedPiece == Piece::WHITE_ROOK &&
        destinationSquare == Square::H1)
        this->whiteCanCastleKingside = false;
    if (capturedPiece == Piece::BLACK_ROOK &&
        destinationSquare == Square::A8)
        this->blackCanCastleQueenside = false;
    if (capturedPiece == Piece::BLACK_ROOK &&
        destinationSquare == Square::H8)
        this->blackCanCastleKingside = false;
}

void Board::undoMove(){
    if (undoStack.empty())
        throw std::logic_error("Cannot undo, stack is empty.");

    auto undoInfo = undoStack.back();
    auto move = undoInfo.move;
    undoStack.pop_back();
    moveHistory.pop_back();

    auto original_square = move.sourceSquare;
    auto original_piece = undoInfo.startingPiece;

    auto updated_square = move.destinationSquare;

    auto old_wk = undoInfo.whiteCanCastleKingside;
    auto old_wq = undoInfo.whiteCanCastleQueenside;
    auto old_bk = undoInfo.blackCanCastleKingside;
    auto old_bq = undoInfo.blackCanCastleQueenside;

    auto enPassentSquare = undoInfo.enPassentSquare;

    removePiece(updated_square);
    placePiece(original_square, original_piece);

    if (move.capturedPiece != Piece::EMPTY)
        placePiece(updated_square, move.capturedPiece);

    whiteCanCastleKingside = old_wk;
    whiteCanCastleQueenside = old_wq;
    blackCanCastleKingside = old_bk;
    blackCanCastleQueenside = old_bq;

    //todo: handle enpassent

    sideToMove = undoInfo.sideToMove;
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

void Board::printBoard() const{
    for (int rank = 7; rank >= 0; --rank){
        std::cout << rank + 1 << " │ ";

        for (int file = 0; file < 8; ++file){
            int index = rank * 8 + file;
            std::cout << printPiece(pieceMap[index]) << " ";
        }

        std::cout << std::endl;
    }
    std::cout << "   ————————————————" << std::endl << "    a b c d e f g h" << std::endl;
}

std::optional<Square> Board::getSquareOffset(Square square, int offsetX, int offsetY){
    const File file = getFile(square);
    const Rank rank = getRank(square);

    int new_file = static_cast<int>(file) + offsetX;
    int new_rank = static_cast<int>(rank) + offsetY;

    if (new_file < 0 || new_file > 7 || new_rank < 0 || new_rank > 7){
        return std::nullopt;
    }
    return static_cast<Square>(new_rank * 8 + new_file);
}

U64* Board::getBitboardPointer(Piece piece){
    U64* bitboard = nullptr;
    switch (piece){
    case Piece::WHITE_PAWN:
        bitboard = &whitePawns;
        break;
    case Piece::WHITE_KNIGHT:
        bitboard = &whiteKnights;
        break;
    case Piece::WHITE_BISHOP:
        bitboard = &whiteBishops;
        break;
    case Piece::WHITE_ROOK:
        bitboard = &whiteRooks;
        break;
    case Piece::WHITE_QUEEN:
        bitboard = &whiteQueens;
        break;
    case Piece::WHITE_KING:
        bitboard = &whiteKing;
        break;

    case Piece::BLACK_PAWN:
        bitboard = &blackPawns;
        break;
    case Piece::BLACK_KNIGHT:
        bitboard = &blackKnights;
        break;
    case Piece::BLACK_BISHOP:
        bitboard = &blackBishops;
        break;
    case Piece::BLACK_ROOK:
        bitboard = &blackRooks;
        break;
    case Piece::BLACK_QUEEN:
        bitboard = &blackQueens;
        break;
    case Piece::BLACK_KING:
        bitboard = &blackKing;
        break;
    case Piece::EMPTY:
        return nullptr;
    }
    return bitboard;
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

std::string Board::printSquare(Square square){
    auto file = getFile(square);
    auto rank = getRank(square);

    std::string file_str;
    const std::string rank_str = std::to_string(static_cast<int>(rank) + 1);

    if (file == File::A) file_str = "A";
    if (file == File::B) file_str = "B";
    if (file == File::C) file_str = "C";
    if (file == File::D) file_str = "D";
    if (file == File::E) file_str = "E";
    if (file == File::F) file_str = "F";
    if (file == File::G) file_str = "G";
    if (file == File::H) file_str = "H";

    return file_str + rank_str;
}
