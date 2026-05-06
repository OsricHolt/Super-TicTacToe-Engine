#ifndef _BOARD_CLASS_
#define _BOARD_CLASS_

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>

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
	smallBitboard xBoardBig;
	smallBitboard oBoardBig;
	int turn;
	bool freeMove; // free move?
	char player;
	int bigBox;
	int smallBox;
	int position;
	DecodedMove decodeMove(int position);
	void makeMove(int position);
	void renderPiece(VAO VAO, int shapeIndex);
	bool isLegalMove(int position);
	Bitboard getLegalMoves();
	void logMove(int position);
	void checkWin(Bitboard& pieceBoard, smallBitboard& pieceBoardBig);
	float mouseX, mouseY;
	float xPieceShift, yPieceShift;
	VAO* VAOx;
	VAO* VAOo;
	int xShapeIndex;
	int oShapeIndex;
	GLuint movePositionShiftUni;

};

#endif