#include <iostream>
#include "board.h"
#include "move.h"

using namespace std;


int main(){
    Board board = Board();
    board.initializeStartingPosition();
    board.printBoard();

    auto moves = generatePawnMoves(board);
    auto knightMoves = generateKnightMoves(board);
    auto bishopMoves = generateBishopMoves(board);
    moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
    moves.insert(moves.end(), knightMoves.begin(), knightMoves.end());


    for (Move move: moves){
        cout << move;
        cout << " - ";
    }
    cout << endl << "Total Moves: " << moves.size() <<endl;

    return 0;
}
