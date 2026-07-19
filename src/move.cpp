//
// Created by Firas on 17/07/2026.
//

#include "move.h"
#include "board.h"
#include <vector>
#include <ostream>
#include <string>


std::vector<Move> generatePawnMoves(Board &board){
    //todo: Make this color-agnostic (make it work for black as well)
    auto legalMoves = std::vector<Move>();
    auto pawns = board.whitePawns;

    for (const auto position : Board::positions){
        if (Board::isBitSet(pawns, position)){ //if pawn in position

            //1. Normal Moves (Up 1, Up 2)
            auto up_one_square = Board::getSquareOffset(position, 0, 1);
            if (up_one_square){    //if not last rank

                //if nothing is directly in front
                if (board.isEmpty(up_one_square.value()))
                    legalMoves.push_back(Move{
                        .sourceSquare = position,
                        .destinationSquare = up_one_square.value(),
                    });

                //if nothing is directly in front, 2 squares in front, AND pawn has not moved before
                if (Board::getRank(position) == Rank::TWO){ //in starting position

                    auto up_two_squares = Board::getSquareOffset(position, 0, 2);
                    if (up_two_squares){

                        if (board.isEmpty(up_one_square.value()) && board.isEmpty(up_two_squares.value()))
                            legalMoves.push_back(Move{
                                .sourceSquare = position,
                                .destinationSquare = up_two_squares.value(),
                            });
                    }
                }
            }

            //2. Diagonal Captures
            auto north_east = Board::getSquareOffset(position, -1, 1);
            if (north_east){
                if (board.isEmpty(north_east.value()) == false){

                    Piece capturingPiece = board.getPiece(position);
                    Piece capturedPiece = board.getPiece(north_east.value());
                    Color capturingPieceColor = Board::getPieceColor(capturingPiece).value();
                    Color capturedPieceColor = Board::getPieceColor(capturedPiece).value();

                    if (capturedPieceColor != capturingPieceColor)
                        legalMoves.push_back(Move{
                            .sourceSquare = position,
                            .destinationSquare = north_east.value(),
                            .capturedPiece = capturedPiece
                        });
                }
            }

            auto south_west = Board::getSquareOffset(position, -1, 1);
            if (south_west){
                if (board.isEmpty(south_west.value()) == false){

                    Piece capturingPiece = board.getPiece(position);
                    Piece capturedPiece = board.getPiece(south_west.value());
                    Color capturingPieceColor = Board::getPieceColor(capturingPiece).value();
                    Color capturedPieceColor = Board::getPieceColor(capturedPiece).value();

                    if (capturedPieceColor != capturingPieceColor)
                    legalMoves.push_back(Move{
                        .sourceSquare = position,
                        .destinationSquare = south_west.value(),
                        .capturedPiece = capturedPiece
                    });
                }
            }

            //3. Promotions
            Rank rank = Board::getRank(position);

            if (rank == Rank::SEVEN){
                up_one_square = Board::getSquareOffset(position, 0, 1);
                if (up_one_square){
                    if (board.isEmpty(up_one_square.value())){
                        auto possiblePromotions = {Piece::WHITE_QUEEN, Piece::WHITE_ROOK, Piece::WHITE_KNIGHT, Piece::WHITE_BISHOP};
                        for (Piece promotionPiece : possiblePromotions){
                            legalMoves.push_back(Move{
                                .sourceSquare = position,
                                .destinationSquare = up_one_square.value(),
                                .promotionPiece = promotionPiece
                            });
                        }
                    }
                }
            }

            //4. Enpassent
            //todo: ADD enpassent

        }
    }

    //END: return
    return legalMoves;
}

// bool isMoveLegal(Move move){
// }

// bool makeMove(Move move){
//     if (isMoveLegal(move)){
//
//     }
// }

std::ostream& operator<<(std::ostream& os, const Move& move){
    return os << "Move [src: " + Board::printSquare(move.sourceSquare) + ", dst: " + Board::printSquare(move.destinationSquare) + "]";
}
