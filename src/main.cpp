#include <iostream>
#include "board.h"
#include "move.h"

using namespace std;


int main(){
    Board board = Board();
    // board.initializeStartingPosition();
    board.setCustomStartingPosition();
    //todo: add test to run moves = expected
    board.printBoard();

    auto moves = generateAllMoves(board);

    for (Move move: moves){
        cout << move;
        cout << " - ";
    }
    cout << endl << "Total Moves: " << moves.size() <<endl;

    return 0;
}
