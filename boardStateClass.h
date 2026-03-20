#ifndef _BOARD_STATE_CLASS_H_
#define _BOARD_STATE_CLASS_H_

#include <glad/glad.h>
#include <bitset>

class BoardState {
	public:
		std::bitset<92> bits;

		void changeBoardState();
		void drawPiece(bool piece);
};



#endif