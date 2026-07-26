#include <iostream>
#include "board.h"
#include "move.h"

using namespace std;


int main(){
    Board board = Board();
    // board.initializeStartingPosition();
    board.setCustomStartingPosition();
    board.printBoard();

    auto moves = generatePawnMoves(board);
    auto knightMoves = generateKnightMoves(board);
    auto bishopMoves = generateBishopMoves(board);
    auto rookMoves = generateRookMoves(board);
    auto queenMoves = generateQueenMoves(board);
    auto kingMoves = generateKingMoves(board);

    moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
    moves.insert(moves.end(), knightMoves.begin(), knightMoves.end());
    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
    moves.insert(moves.end(), queenMoves.begin(), queenMoves.end());
    moves.insert(moves.end(), kingMoves.begin(), kingMoves.end());

    for (Move move: moves){
        cout << move;
        cout << " - ";
    }
    cout << endl << "Total Moves: " << moves.size() <<endl;

    return 0;
}
