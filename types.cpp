#include <iostream>
#include "types.h"

Board::Board() {
	Board.xBoard1 = 0U;
}



Bitboard* xBoard1; // X positions 0 - 62 of the board 
halfBitboard* xBoard2; // X positions 63 - 89 of the board
Bitboard* oBoard1; // O positions 0 - 62 of the board 
halfBitboard* oBoard2; // O positions 63 - 89 of the board
Bitboard* openSpots1;
halfBitboard* openSpots2;
int turn;
int freeMove; // free move?
