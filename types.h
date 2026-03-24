#ifndef _TYPES_H_
#define _TYPES_H_

#include <glad/glad.h>
#include <stdint.h>

typedef uint64_t Bitboard;
typedef uint32_t halfBitboard;
class Board {
	public:
		Board();
		Bitboard xBoard1; // X positions 0 - 62 of the board 
		halfBitboard xBoard2; // X positions 63 - 89 of the board
		Bitboard oBoard1; // O positions 0 - 62 of the board 
		halfBitboard oBoard2; // O positions 63 - 89 of the board
		Bitboard openSpots1;
		halfBitboard openSpots2;
		int turn;
		int freeMove; // free move?
};




#endif