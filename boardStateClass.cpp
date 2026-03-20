#include "boardStateClass.h"

class BoardState {
public:
	std::bitset<92> bits;

	void changeBoardState();
	void drawPiece(bool piece);
};
