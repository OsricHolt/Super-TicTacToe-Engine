#ifndef _TYPES_H_
#define _TYPES_H_

#include <glad/glad.h>
#include <stdint.h>
#include <iostream>


struct Bitboard {
	uint64_t low;
	uint64_t high;
};
struct DecodedMove {
	int boardHalf;
	int moveIndex;
};

inline constexpr Bitboard VALID_SPACES = { ~0ULL, (1ULL << 17) - 1}; // Set the valid spaces to prepare valid moves bitboard

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