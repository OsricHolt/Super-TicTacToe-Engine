#include "boardClass.h"

Board::Board() {
	// initialize all variables
	WIDTH = 800;
	HEIGHT = 800;
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
