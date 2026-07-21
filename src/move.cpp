//
// Created by Firas on 17/07/2026.
//

#include "move.h"
#include "board.h"
#include <vector>
#include <array>
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

std::vector<Move> generateKnightMoves(Board &board){
    //todo: make this color-agnostic also
    auto legalMoves = std::vector<Move>();
    auto knights = board.whiteKnights;

    for (const auto position : Board::positions){
        if (Board::isBitSet(knights, position)){ // if knight is in position
            const auto destinations = getPossibleKnightDestinations(position);
            for (const auto destination : destinations){
                // Case 1. Empty Square
                if (board.isEmpty(destination)){
                    legalMoves.push_back(Move{
                        .sourceSquare = position,
                        .destinationSquare = destination
                    });
                    continue;
                }
                // Case 2. Capture (Opposite color)
                const Piece capturingPiece = board.getPiece(position);
                const Piece capturedPiece = board.getPiece(destination);
                const Color capturingPieceColor = Board::getPieceColor(capturingPiece).value();
                const Color capturedPieceColor = Board::getPieceColor(capturedPiece).value();

                if (capturedPieceColor != capturingPieceColor){
                    legalMoves.push_back(Move{
                        .sourceSquare = position,
                        .destinationSquare = destination,
                        .capturedPiece = capturedPiece
                    });
                }
            }
        }
    }
    return legalMoves;
}

std::vector<Square> getPossibleKnightDestinations(Square square){
    std::vector<std::pair<int, int>> offsets = {
        { 1,  2},
        { 2,  1},
        { 2, -1},
        { 1, -2},
        {-1, -2},
        {-2, -1},
        {-2,  1},
        {-1,  2}
    };
    std::vector<Square> destinations;
    for (auto offset : offsets){
        auto destinationSquare = Board::getSquareOffset(square, offset.first, offset.second);
        if (destinationSquare){
            destinations.push_back(destinationSquare.value());
        }
    }
    return destinations;
}

std::vector<Move> generateBishopMoves(Board& board){
    //todo: make color-agnostic
    auto legalMoves = std::vector<Move>();
    auto bishops = board.whiteBishops;

    for (const auto position : Board::positions){
        if (Board::isBitSet(bishops, position)){ //if bishop in position
            //todo: here we can pass the legalmoves by reference later on to squeeze out more performance instead of temp vectors
            auto currBishopMoves = getDiagonalMoves(board, position);
            legalMoves.insert(legalMoves.end(), currBishopMoves.begin(), currBishopMoves.end());
        }
    }
    return legalMoves;
}

std::vector<Move> getDiagonalMoves(Board& board, Square square){
    std::vector<Move> diagonalMoves{};
    const Color currentPieceColor = Board::getPieceColor(board.getPiece(square)).value();

    // for each direction
    for (int i = 0; i < 4; i++){
        // for each distance (1-7)
        for (int j = 1; j <= 7; j++){

            int x; int y;
            if (i == 0) {x = j; y = j;}   //NE
            if (i == 1) {x = j; y = -j;}  //SE
            if (i == 2) {x = -j; y = -j;} //SW
            if (i == 3) {x = -j; y = j;}  //NW

            auto destinationSquare = Board::getSquareOffset(square, x, y);
            if (destinationSquare){
                //1. Capture / Friendly Piece
                if (board.isOccupied(destinationSquare.value())){ // if there's a piece
                    // check if enemy or same color
                    const Piece capturedPiece = board.getPiece(destinationSquare.value());
                    const Color capturedPieceColor = Board::getPieceColor(capturedPiece).value();

                    // if same color, prune the branch
                    if (currentPieceColor == capturedPieceColor) break;

                    // if different, add it as a legal move then prune the branch
                    diagonalMoves.push_back(Move{
                        .sourceSquare = square,
                        .destinationSquare = destinationSquare.value(),
                        .capturedPiece = capturedPiece
                    });
                    break;

                }
                // if the square is empty
                else{
                    diagonalMoves.push_back(Move{
                        .sourceSquare = square,
                        .destinationSquare = destinationSquare.value()
                    });
                }
            }
        }
    }
    return diagonalMoves;
}


std::vector<Move> generateRookMoves(Board& board){
    //todo: make color-agnostic
    auto legalMoves = std::vector<Move>();
    auto rooks = board.whiteRooks;

    for (const auto position : Board::positions){
        if (Board::isBitSet(rooks, position)){ //if rook in position
            //todo: here we can pass the legalmoves by reference later on to squeeze out more performance instead of temp vectors
            auto currRookMoves = getStraightMoves(board, position);
            legalMoves.insert(legalMoves.end(), currRookMoves.begin(), currRookMoves.end());
        }
    }
    return legalMoves;
}

std::vector<Move> getStraightMoves(Board& board, Square square){
    std::vector<Move> straightMoves{};
    const Color currentPieceColor = Board::getPieceColor(board.getPiece(square)).value();

    // for each direction
    for (int i = 0; i < 4; i++){
        // for each distance (1-7)
        for (int j = 1; j <= 7; j++){

            int x; int y;
            if (i == 0) {x = 0; y = j;}  //N
            if (i == 1) {x = j; y = 0;}  //E
            if (i == 2) {x = 0; y = -j;} //S
            if (i == 3) {x = -j; y = 0;} //W

            auto destinationSquare = Board::getSquareOffset(square, x, y);
            if (destinationSquare){

                //1. Capture / Friendly Piece
                if (board.isOccupied(destinationSquare.value())){ // if there's a piece
                    // check if enemy or same color
                    const Piece capturedPiece = board.getPiece(destinationSquare.value());
                    const Color capturedPieceColor = Board::getPieceColor(capturedPiece).value();

                    // if same color, prune the branch
                    if (currentPieceColor == capturedPieceColor) break;

                    // if different, add it as a legal move then prune the branch
                    straightMoves.push_back(Move{
                        .sourceSquare = square,
                        .destinationSquare = destinationSquare.value(),
                        .capturedPiece = capturedPiece
                    });
                    break;

                }
                // if the square is empty
                else{
                    straightMoves.push_back(Move{
                        .sourceSquare = square,
                        .destinationSquare = destinationSquare.value()
                    });
                }
            }
        }
    }
}


std::ostream& operator<<(std::ostream& os, const Move& move){
    return os << "Move [src: " + Board::printSquare(move.sourceSquare) + ", dst: " + Board::printSquare(move.destinationSquare) + "]";
}
