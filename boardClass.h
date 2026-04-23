#ifndef _BOARD_CLASS_
#define _BOARD_CLASS_

#include <iostream>
#include <fstream>
#include <string>

#include "types.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


class Board {
public:
	Board();
	int WIDTH;
	int HEIGHT;
	Bitboard xBoard;
	Bitboard oBoard;
	int turn;
	bool freeMove; // free move?
	char player;
	int bigBox;
	int smallBox;
	DecodedMove decodeMove(int position);
	void MakeMove(int position);
	void renderPiece(VAO VAO, int shapeIndex);
	bool IsLegalMove(int position);
	Bitboard GetLegalMoves();
	float mouseX, mouseY;
	float xPieceShift, yPieceShift;
	VAO* VAOx;
	VAO* VAOo;
	int xShapeIndex;
	int oShapeIndex;
	GLuint movePositionShiftUni;

};

#endif