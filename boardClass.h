#ifndef _BOARD_CLASS_
#define _BOARD_CLASS_


class Board {
public:
	Board();
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
};

Board::Board() {
	// initialize all variables to 0
	xBoard1 = 0;
	xBoard2 = 0;
	oBoard1 = 0;
	oBoard2 = 0;
	openSpots1 = 0;
	openSpots2 = 0;
	turn = 0;
	freeMove = true;
	player = 'X';
	bigBox = 5;
	smallBox = 5;
}
bool Board::IsLegalMove(int position) {
	if (position < 0 || position > 80) {
		return false;
	}
	if (position <= 63) {
		if ((1ULL << position) & openSpots1) {
			return false;
		}
	}
	else {
		if ((1ULL << (position - 64)) & openSpots2) {
			return false;
		}
	}
	return true;
}
void Board::MakeMove(int position) {
	//		Check if move is on board	//		Check if the position is in the first bitmap and if it is open // check if the second bit map is open
	if (!IsLegalMove(position)) { // check if legal move
		std::cout << "Please input valid move" << std::endl;
		return;
	}
	if (turn % 2 == 0) { // check move parity; player X move sequence
		if (position <= 63) { // check for first half of board
			xBoard1 |= (1ULL << position);
		}
		else {
			xBoard2 |= (1ULL << (position - 64));
		}
		player = 'X';
	}
	else {
		if (position <= 63) { // check for first half of board
			oBoard1 |= (1ULL << position);
		}
		else {
			oBoard2 |= (1ULL << (position - 64));
		}
		player = 'O';
	}
	if (position <= 63) { // check for first half of board
		openSpots1 |= (1ULL << position);
	}
	else {
		openSpots2 |= (1ULL << (position - 64));
	}
	bigBox = (int)(position / 9); // translate integer move position to player notation for big box
	smallBox = position % 9;	   // translate integer move position to player notation for small box
	std::cout << "Move " << (turn + 1) << ": " << player << " [ " << bigBox << ", " << smallBox << "]" << std::endl; // Print move to log in player notation
	turn += 1;
}

#endif