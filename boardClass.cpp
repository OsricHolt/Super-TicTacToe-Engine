#include "boardClass.h"

Board::Board() {
	// initialize all variables
	WIDTH = 800;
	HEIGHT = 800;
	xBoard = { 0, 0 };
	oBoard = { 0, 0 };
	xBoardBig = 0;
	oBoardBig = 0;
	turn = 0;
	freeMove = true;
	lastMove = 0;
	player = 'X';
	bigBox = 5;
	smallBox = 5;
	position = 0;
	mouseX = 0;
	mouseY = 0;
	xPieceShift = 0;
	yPieceShift = 0;
	VAOx = nullptr;
	VAOo = nullptr;
	xShapeIndex = 0;
	oShapeIndex = 0;
	movePositionShiftUni = 0;

}

DecodedMove Board::decodeMove(int position) {
	DecodedMove decodeValues = { (position >> 6), (position & 63) }; // {board half index (int cast position / 64), adjusted index (fast position % 64)}
	return decodeValues;
}
bool Board::isLegalMove(int position) {
	Bitboard legalMoves = getLegalMoves();
	DecodedMove decodeValues = decodeMove(position);
	if (decodeValues.boardHalf > 1) return false; // check if position is on the bitboard
	if ((xBoardBig | oBoardBig) & (1 << (position / 9))) return false; // check if the big box to move in is taken
	if ((position / 9) != (lastMove % 9)) { // check if current move big box matches old move big box
		if (freeMove == false) {
			return false;
		}
	}
	uint64_t legalHalf = (decodeValues.boardHalf == 0) // fix the correct half of the board
		? legalMoves.low
		: legalMoves.high;
	return (legalHalf & (1ULL << decodeValues.moveIndex)) != 0; // check to see if the move is legal
	return false;
	}

Bitboard Board::getLegalMoves() {
	Bitboard legalMoves = {0, 0};
	legalMoves.low = ~(xBoard.low | oBoard.low) & VALID_SPACES.low; // complement of all taken spaces and limited to spaces on the board for first half of board
	legalMoves.high = ~(xBoard.high | oBoard.high) & VALID_SPACES.high; // complement of all taken spaces and limited to spaces on the board for second half of board
	return legalMoves;
}

void Board::makeMove(int position) {
	//		Check if move is on board	//		Check if the position is in the first bitmap and if it is open // check if the second bit map is open
	if (!isLegalMove(position)) { // check if legal move
		std::cout << "Please input valid move" << std::endl;
		return;
	}
	freeMove = false;
	DecodedMove decodeValues = decodeMove(position); // translate position into board half and move index
	player = (turn & 1) // fix the active player's name (conditdion is backwards because
		? 'O'
		: 'X';
	Bitboard& playerBoard = (turn & 1) // fix the active player's board
		? oBoard
		: xBoard;
	smallBitboard& playerBoardBig = (turn & 1) // fix the active player's board
		? oBoardBig
		: xBoardBig;
	uint64_t& halfBoard = (decodeValues.boardHalf == 0) // fix the right half of the board
		? playerBoard.low
		: playerBoard.high;
	halfBoard |= (1ULL << decodeValues.moveIndex); // occupy the active player's bitboard space with their move

	checkWin(playerBoard, playerBoardBig);
	if ((xBoardBig | oBoardBig) & (1 << (position % 9))) freeMove = true; // check if the big box to move in is taken
	lastMove = position;
	turn += 1;
}

void Board::checkWin(Bitboard& pieceBoard, smallBitboard& pieceBoardBig) {
	int position = 0;
	uint16_t tempBoard = 0;
	uint64_t halfBoard = 0;
	int boardHalf = 0;
	int bit = 0;
	// store each subgrid in the temporary board
	for (int i = 0; i < 9; i++) { // small grid index
		if (pieceBoardBig & (1ULL << i)) continue; // checks to see if the box is already taken
		// populate the temporary board for win checks
		for (int j = 0; j < 9; j++) { // iterate over each small grid position
			position = (i * 9) + j; // translate loops to position
			boardHalf = position >> 6;
			bit = position & 63;
			halfBoard = (boardHalf == 0) ? pieceBoard.low : pieceBoard.high; // fix bitboard half
			if (halfBoard & (1ULL << bit)) { // check if position bit is set
				tempBoard |= (1 << j); // store flipped bit in small grid copy
				}
			//std::cout << tempBoard << std::endl;
			//tempBoard &= (1 << 9) - 1;
		}
		// check for board win
		// iterate over all winning positions
		for (int k = 0; k < 8; k++) {
			if ((tempBoard & WIN_MASKS[k]) == WIN_MASKS[k]) {
				pieceBoardBig |= 1ULL << i; // set big box to won
				std::cout << std::bitset<9>(pieceBoardBig) << std::endl;
				break;
			}
		}
		std::cout << tempBoard << std::endl;

		tempBoard = 0;
	}
	// mask with 8 win arrays
	// store box is taken if taken
	//
}

void Board::logMove(int position) {
	bigBox = (int)(position / 9); // translate integer move position to player notation for big box
	smallBox = position % 9;	   // translate integer move position to player notation for small box
	//std::cout << xPieceShift << " , " << yPieceShift << std::endl;

	std::ofstream gameLog;
	gameLog.open("Games/game.txt", std::ios::app);
	gameLog << "Move " << (turn + 1) << ": " << player << " [ " << bigBox << ", " << smallBox << "]\n"; // Print move to log in player notation
	gameLog.close();

}
void Board::renderPiece(VAO VAO, int shapeIndex) {
	VAO.Bind();

	GLint ebo = 0;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo);

	if (ebo != 0) glDrawElements(GL_TRIANGLES, shapeIndex, GL_UNSIGNED_INT, 0);
	else glDrawArrays(GL_TRIANGLE_STRIP, 0, shapeIndex);
	VAO.Unbind();
}
