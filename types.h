#ifndef _TYPES_H_
#define _TYPES_H_

#include <glad/glad.h>
#include <stdint.h>
#include <iostream>

typedef uint16_t smallBitboard;
struct Bitboard {
	uint64_t low;
	uint64_t high;
};
struct DecodedMove {
	int boardHalf;
	int moveIndex;
};

inline constexpr Bitboard VALID_SPACES = { ~0ULL, (1ULL << 17) - 1}; // Set the valid spaces to prepare valid moves bitboard
inline constexpr uint16_t WIN_MASKS[8] = {
	0b111000000, // top row
	0b000111000, // middle row
	0b000000111, // bottom row
	0b100010001, // backslash diagonal
	0b001010100, // forward slash diagonal
	0b100100100, // left column
	0b010010010, // middle colunm
	0b001001001  // right column
};

inline int ctz64(uint64_t x) { // portable count trailing zeros function
#ifdef _MSC_VER 
	unsigned long i; 
	return _BitScanForward64(&i, x) 
		? (int)i 
		: -1; 
#else return x 
	? __builtin_ctzll(x) 
	: -1; 
#endif 
}

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


#endif