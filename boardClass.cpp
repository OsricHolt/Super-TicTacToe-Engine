#include "boardClass.h"

Board::Board() {
	// initialize all variables
	WIDTH = 800;
	HEIGHT = 800;
	xBoard = { 0, 0 };
	oBoard = { 0, 0 };
	turn = 0;
	freeMove = true;
	player = 'X';
	bigBox = 5;
	smallBox = 5;
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
	DecodedMove decodeValues = { (position >> 6), (position & 63)};
	return decodeValues;
}
bool Board::IsLegalMove(int position) {
	Bitboard legalMoves = GetLegalMoves();
	DecodedMove decodeValues = decodeMove(position);
	if (decodeValues.boardHalf > 1) return false; // check if position is on the bitboard
	uint64_t legalHalf = (decodeValues.boardHalf == 0) // fix the right half of the board
		? legalMoves.low
		: legalMoves.high;
	return (legalHalf & (1ULL << decodeValues.moveIndex)) != 0; // check to see if the move is legal
	return false;
	}

Bitboard Board::GetLegalMoves() {
	Bitboard legalMoves = {0, 0};
	legalMoves.low = ~(xBoard.low | oBoard.low) & VALID_SPACES.low; // complement of all taken spaces and limited to spaces on the board for first half of board
	legalMoves.high = ~(xBoard.high | oBoard.high) & VALID_SPACES.high; // complement of all taken spaces and limited to spaces on the board for second half of board
	return legalMoves;
}

void Board::MakeMove(int position) {
	//		Check if move is on board	//		Check if the position is in the first bitmap and if it is open // check if the second bit map is open
	if (!IsLegalMove(position)) { // check if legal move
		std::cout << "Please input valid move" << std::endl;
		return;
	}
	DecodedMove decodeValues = decodeMove(position); // translate position into board half and move index
	player = (turn & 1)
		? 'X'
		: 'O';
	Bitboard* playerBoard = (turn & 1)
		? &xBoard
		: &oBoard;
	uint64_t* halfBoard = (decodeValues.boardHalf == 0) // fix the right half of the board
		? &playerBoard->low
		: &playerBoard->high;
	*halfBoard |= (1ULL << decodeValues.moveIndex);

	bigBox = (int)(position / 9); // translate integer move position to player notation for big box
	smallBox = position % 9;	   // translate integer move position to player notation for small box
	xPieceShift = WIDTH * ((bigBox % 3) / 3.0f + (smallBox % 3) / 9.0f);
	yPieceShift = -HEIGHT * ((int)(bigBox / 3) / 3.0f + (int)(smallBox / 3) / 9.0f);
	//std::cout << xPieceShift << " , " << yPieceShift << std::endl;
	glUniform2f(movePositionShiftUni, xPieceShift, yPieceShift);

	if (player == 'X') {
		renderPiece(*VAOx, xShapeIndex);
	}
	else if (player == 'O') {
		renderPiece(*VAOo, oShapeIndex);
	}

	std::ofstream gameLog;
	gameLog.open("Games/game.txt", std::ios::app);
	gameLog << "Move " << (turn + 1) << ": " << player << " [ " << bigBox << ", " << smallBox << "]\n"; // Print move to log in player notation
	gameLog.close();
	turn += 1;
}

void Board::renderPiece(VAO VAO, int shapeIndex) {
	VAO.Bind();

	GLint ebo = 0;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo);

	if (ebo != 0) glDrawElements(GL_TRIANGLES, shapeIndex, GL_UNSIGNED_INT, 0);
	else glDrawArrays(GL_TRIANGLE_STRIP, 0, shapeIndex);
	VAO.Unbind();
}
