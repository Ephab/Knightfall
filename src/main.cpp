#include <iostream>
#include "board.h"
#include "move.h"

using namespace std;


int main(){
    Board board = Board();
    board.initializeStartingPosition();
    board.printBoard();

    auto moves = generatePawnMoves(board);
    for (Move move: moves){
        cout << move << endl;
    }

    return 0;
}
