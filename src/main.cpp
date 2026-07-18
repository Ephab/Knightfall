#include <iostream>
#include "board.h"


int main(){
    Board board = Board();
    board.initializeStartingPosition();
    board.printBoard();
    return 0;
}
