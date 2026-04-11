#ifndef _BOARD_CLASS_
#define _BOARD_CLASS_

#include <iostream>
#include <fstream>
#include <string>

#include "types.h"

class Board {
public:
	Board();
	int WIDTH;
	int HEIGHT;
	Bitboard xBoard1; // X positions 0 - 62 of the board 
	halfBitboard xBoard2; // X positions 63 - 89 of the board
	Bitboard oBoard1; // O positions 0 - 62 of the board 
	halfBitboard oBoard2; // O positions 63 - 89 of the board
	Bitboard openSpots1; // free spots positions 0 - 62 of the board
	halfBitboard openSpots2; // free spots positions 63 - 89 of the board
	int turn;
	bool freeMove; // free move?
	char player;
	int bigBox;
	int smallBox;
	void MakeMove(int position);
	bool IsLegalMove(int position);
	float mouseX, mouseY;
};

#endif