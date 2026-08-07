//
// Created by Firas on 17/07/2026.
//

#include "move.h"
#include "board.h"
#include <vector>
#include <array>
#include <map>
#include <functional>
#include <numbers>
#include <ostream>
#include <string>
#include <unordered_map>

/*todo:
    promotions
    en passant
    castling
    color-agnostic,
    filtering out moves that leave king in check.
*/

std::vector<Move> generatePawnMoves(Board& board, Square pawnSquare, bool white){
    //todo: finish EnPassent.

    int one_square_y;
    int two_square_y;
    int ne_x; int ne_y;
    int nw_x; int nw_y;
    Rank startingRank;
    Rank promotionRank;
    std::array<Piece, 4> possiblePromotions;

    switch (white){
        case true:
            one_square_y = 1;
            two_square_y = 2;
            ne_x = 1; ne_y = 1;
            nw_x = -1; nw_y = 1;
            startingRank = Rank::TWO;
            promotionRank = Rank::SEVEN;
            possiblePromotions = {Piece::WHITE_QUEEN, Piece::WHITE_KNIGHT, Piece::WHITE_BISHOP, Piece::WHITE_ROOK};
            break;

        case false:
            one_square_y = -1;
            two_square_y = -2;
            ne_x = -1; ne_y = -1;
            nw_x = 1; nw_y = -1;
            startingRank = Rank::SEVEN;
            promotionRank = Rank::TWO;
            possiblePromotions = {Piece::BLACK_QUEEN, Piece::BLACK_KNIGHT, Piece::BLACK_BISHOP, Piece::BLACK_ROOK};
            break;
    }

    auto legalMoves = std::vector<Move>();

    //1. Normal Moves (Up 1, Up 2)
    auto up_one_square = Board::getSquareOffset(pawnSquare, 0, one_square_y); //(0, 1/-1)
    if (up_one_square){ //if legal rank

        //if nothing is directly in front
        //if you're on rank 7, you MUST promote! you cannot just do a normal 1 square move
        if (board.isEmpty(up_one_square.value()) && Board::getRank(pawnSquare) != promotionRank)
            legalMoves.push_back(Move{
                .sourceSquare = pawnSquare,
                .destinationSquare = up_one_square.value(),
            });

        //if nothing is directly in front, 2 squares in front, AND pawn has not moved before
        if (Board::getRank(pawnSquare) == startingRank){ //in starting position

            auto up_two_squares = Board::getSquareOffset(pawnSquare, 0, two_square_y); //(0, 2/-2)
            if (up_two_squares){
                if (board.isEmpty(up_one_square.value()) && board.isEmpty(up_two_squares.value()))
                    legalMoves.push_back(Move{
                        .sourceSquare = pawnSquare,
                        .destinationSquare = up_two_squares.value(),
                    });
            }
        }
    }

    //2. Diagonal Captures
    auto north_east = Board::getSquareOffset(pawnSquare, ne_x, ne_y); // (1, 1) / (-1, -1)
    if (north_east){
        if (board.isEmpty(north_east.value()) == false){

            Piece capturingPiece = board.getPiece(pawnSquare);
            Piece capturedPiece = board.getPiece(north_east.value());
            Color capturingPieceColor = Board::getPieceColor(capturingPiece).value();
            Color capturedPieceColor = Board::getPieceColor(capturedPiece).value();

            if (capturedPieceColor != capturingPieceColor){
                if (Board::getRank(pawnSquare) == promotionRank){
                    for (auto promotionPiece : possiblePromotions){
                        legalMoves.push_back(Move{
                            .sourceSquare = pawnSquare,
                            .destinationSquare = north_east.value(),
                            .promotionPiece = promotionPiece,
                            .capturedPiece = capturedPiece
                        });
                    }
                }
                else{
                    legalMoves.push_back(Move{
                        .sourceSquare = pawnSquare,
                        .destinationSquare = north_east.value(),
                        .capturedPiece = capturedPiece
                    });
                }
            }
        }
    }

    auto north_west = Board::getSquareOffset(pawnSquare, nw_x, nw_y); // (-1, 1) / (1, -1)
    if (north_west){
        if (board.isEmpty(north_west.value()) == false){

            Piece capturingPiece = board.getPiece(pawnSquare);
            Piece capturedPiece = board.getPiece(north_west.value());
            Color capturingPieceColor = Board::getPieceColor(capturingPiece).value();
            Color capturedPieceColor = Board::getPieceColor(capturedPiece).value();

            if (capturedPieceColor != capturingPieceColor){
                if (Board::getRank(pawnSquare) == promotionRank){
                    for (auto promotionPiece : possiblePromotions){
                        legalMoves.push_back(Move{
                            .sourceSquare = pawnSquare,
                            .destinationSquare = north_west.value(),
                            .promotionPiece = promotionPiece,
                            .capturedPiece = capturedPiece
                        });
                    }
                }
                else{
                    legalMoves.push_back(Move{
                        .sourceSquare = pawnSquare,
                        .destinationSquare = north_west.value(),
                        .capturedPiece = capturedPiece
                    });
                }

            }
        }
    }

    //3. Promotions
    Rank rank = Board::getRank(pawnSquare);

    if (rank == promotionRank){
        up_one_square = Board::getSquareOffset(pawnSquare, 0, one_square_y); // (0, 1/-1)
        if (up_one_square){
            if (board.isEmpty(up_one_square.value())){
                for (Piece promotionPiece : possiblePromotions){
                    legalMoves.push_back(Move{
                        .sourceSquare = pawnSquare,
                        .destinationSquare = up_one_square.value(),
                        .promotionPiece = promotionPiece
                    });
                }
            }
        }
    }

    //4. Enpassent
    //todo: ADD enpassent

    //5. END
    return legalMoves;
}

std::vector<Move> generateKnightMoves(Board& board, Square knightSquare, bool white){
    //todo: make this color-agnostic also
    auto legalMoves = std::vector<Move>();
    const auto destinations = getPossibleKnightDestinations(knightSquare);
    for (const auto destination : destinations){
        // Case 1. Empty Square
        if (board.isEmpty(destination)){
            legalMoves.push_back(Move{
                .sourceSquare = knightSquare,
                .destinationSquare = destination
            });
            continue;
        }
        // Case 2. Capture (Opposite color)
        const Piece capturingPiece = board.getPiece(knightSquare);
        const Piece capturedPiece = board.getPiece(destination);
        const Color capturingPieceColor = Board::getPieceColor(capturingPiece).value();
        const Color capturedPieceColor = Board::getPieceColor(capturedPiece).value();

        if (capturedPieceColor != capturingPieceColor){
            legalMoves.push_back(Move{
                .sourceSquare = knightSquare,
                .destinationSquare = destination,
                .capturedPiece = capturedPiece
            });
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

std::vector<Move> generateBishopMoves(Board& board, Square bishopSquare, bool white){
    //todo: make color-agnostic
    auto legalMoves = std::vector<Move>();

    //todo: here we can pass the legalmoves by reference later on to squeeze out more performance instead of temp vectors
    auto currBishopMoves = getDiagonalMoves(board, bishopSquare);
    legalMoves.insert(legalMoves.end(), currBishopMoves.begin(), currBishopMoves.end());

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

std::vector<Move> generateRookMoves(Board& board, Square rookSquare, bool white){
    //todo: make color-agnostic
    auto legalMoves = std::vector<Move>();

    //todo: here we can pass the legalmoves by reference later on to squeeze out more performance instead of temp vectors
    auto currRookMoves = getStraightMoves(board, rookSquare);
    legalMoves.insert(legalMoves.end(), currRookMoves.begin(), currRookMoves.end());

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
    return straightMoves;
}

std::vector<Move> generateQueenMoves(Board& board, Square queenSquare, bool white){
    //todo: make color-agnostic
    auto legalMoves = std::vector<Move>();

    //todo: here we can pass the legalmoves by reference later on to squeeze out more performance instead of temp vectors
    auto straightQueenMoves = getStraightMoves(board, queenSquare);
    auto diagonalQueenMoves = getDiagonalMoves(board, queenSquare);
    legalMoves.insert(legalMoves.end(), straightQueenMoves.begin(), straightQueenMoves.end());
    legalMoves.insert(legalMoves.end(), diagonalQueenMoves.begin(), diagonalQueenMoves.end());

    return legalMoves;
}

std::vector<Move> generateKingMoves(Board& board, Square kingSquare, bool white){
    //todo: make this color-agnostic also
    auto legalMoves = std::vector<Move>();

    const auto destinations = getPossibleKingDestinations(kingSquare);
    for (const auto destination : destinations){

        // Case 1. Empty Square
        if (board.isEmpty(destination)){
            legalMoves.push_back(Move{
                .sourceSquare = kingSquare,
                .destinationSquare = destination
            });
            continue;
        }
        // Case 2. Capture (Opposite color)
        const Piece capturingPiece = board.getPiece(kingSquare);
        const Piece capturedPiece = board.getPiece(destination);
        const Color capturingPieceColor = Board::getPieceColor(capturingPiece).value();
        const Color capturedPieceColor = Board::getPieceColor(capturedPiece).value();

        if (capturedPieceColor != capturingPieceColor){
            legalMoves.push_back(Move{
                .sourceSquare = kingSquare,
                .destinationSquare = destination,
                .capturedPiece = capturedPiece
            });
        }
    }
    return legalMoves;
}

std::vector<Square> getPossibleKingDestinations(Square square){
    std::vector<std::pair<int, int>> offsets = {
        { 0, 1},  //N
        { 1, 0},  //E
        { 0, -1}, //S
        {-1, 0},  //W
        { 1, 1},  //NE
        { 1, -1}, //SE
        {-1, -1}, //SW
        {-1,  1}  //NW
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

std::vector<Move> generateAllMoves(Board& board){
    //todo: check color for generation
    std::vector<Move> allMoves;
    bool white = board.sideToMove == Color::WHITE;

    std::vector<std::pair<Piece, U64>> pieceBitboardMap = {
        {Piece::WHITE_PAWN, board.whitePawns},
        {Piece::WHITE_KNIGHT, board.whiteKnights},
        {Piece::WHITE_BISHOP, board.whiteBishops},
        {Piece::WHITE_ROOK, board.whiteRooks},
        {Piece::WHITE_QUEEN, board.whiteQueens},
        {Piece::WHITE_KING, board.whiteKing},
        {Piece::BLACK_PAWN, board.blackPawns},
        {Piece::BLACK_KNIGHT, board.blackKnights},
        {Piece::BLACK_BISHOP, board.blackBishops},
        {Piece::BLACK_ROOK, board.blackRooks},
        {Piece::BLACK_QUEEN, board.blackQueens},
        {Piece::BLACK_KING, board.blackKing}
    };

    std::vector<Move> pawnMoves{};
    std::vector<Move> knightMoves{};
    std::vector<Move> bishopMoves{};
    std::vector<Move> rookMoves{};
    std::vector<Move> queenMoves{};
    std::vector<Move> kingMoves{};

    for (auto [piece, bitboard] : pieceBitboardMap){
        for (auto position : Board::positions){
            if (!Board::isBitSet(bitboard, position))
                continue;

            std::vector<Move> generatedMoves;

            switch (piece){
                case Piece::WHITE_PAWN:
                case Piece::BLACK_PAWN:
                    generatedMoves = generatePawnMoves(board, position, white);
                    pawnMoves.insert(
                        pawnMoves.end(),
                        generatedMoves.begin(),
                        generatedMoves.end()
                    );
                    break;

                case Piece::WHITE_KNIGHT:
                case Piece::BLACK_KNIGHT:
                    generatedMoves = generateKnightMoves(board, position, white);
                    knightMoves.insert(
                        knightMoves.end(),
                        generatedMoves.begin(),
                        generatedMoves.end()
                    );
                    break;

                case Piece::WHITE_BISHOP:
                case Piece::BLACK_BISHOP:
                    generatedMoves = generateBishopMoves(board, position, white);
                    bishopMoves.insert(
                        bishopMoves.end(),
                        generatedMoves.begin(),
                        generatedMoves.end()
                    );
                    break;

                case Piece::WHITE_ROOK:
                case Piece::BLACK_ROOK:
                    generatedMoves = generateRookMoves(board, position, white);
                    rookMoves.insert(
                        rookMoves.end(),
                        generatedMoves.begin(),
                        generatedMoves.end()
                    );
                    break;

                case Piece::WHITE_QUEEN:
                case Piece::BLACK_QUEEN:
                    generatedMoves = generateQueenMoves(board, position, white);
                    queenMoves.insert(
                        queenMoves.end(),
                        generatedMoves.begin(),
                        generatedMoves.end()
                    );
                    break;

                case Piece::WHITE_KING:
                case Piece::BLACK_KING:
                    generatedMoves = generateKingMoves(board, position, white);
                    kingMoves.insert(
                        kingMoves.end(),
                        generatedMoves.begin(),
                        generatedMoves.end()
                    );
                    break;

                case Piece::EMPTY:
                    break;
            }
        }
    }

    allMoves.insert(allMoves.end(), pawnMoves.begin(), pawnMoves.end());
    allMoves.insert(allMoves.end(), knightMoves.begin(), knightMoves.end());
    allMoves.insert(allMoves.end(), bishopMoves.begin(), bishopMoves.end());
    allMoves.insert(allMoves.end(), rookMoves.begin(), rookMoves.end());
    allMoves.insert(allMoves.end(), queenMoves.begin(), queenMoves.end());
    allMoves.insert(allMoves.end(), kingMoves.begin(), kingMoves.end());

    return allMoves;
}


std::ostream& operator<<(std::ostream& os, const Move& move){
    return os << "Move [src: " + Board::printSquare(move.sourceSquare) + ", dst: " + Board::printSquare(move.destinationSquare) + "]";
}
