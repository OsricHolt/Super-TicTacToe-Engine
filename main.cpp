#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "types.h"
#include "boardClass.h"
#include "shaderClass.h"

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"



void renderPiece(VAO VAO, int shapeIndex) {
	VAO.Bind();

	GLint ebo = 0;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo);

	if (ebo != 0) glDrawElements(GL_TRIANGLES, shapeIndex, GL_UNSIGNED_INT, 0);
	else glDrawArrays(GL_TRIANGLE_STRIP, 0, shapeIndex);
	VAO.Unbind();
}

void renderGameState(Board board, VAO VAOx, VAO VAOo, VAO VAOgrid, GLuint movePositionShiftUni, GLuint pieceColorUni) {
	Bitboard xBoard = board.xBoard; // copy of the X bitboard
	Bitboard oBoard = board.oBoard; // copy of the O bitboard
	int leastSiginificanBit = 0;
	int piecePosition = 0;
	float xPieceShift = 0.0f;
	float yPieceShift = 0.0f;
	uint64_t* boardCopy = nullptr;
	int positionOffset = 0;
	glUniform2f(movePositionShiftUni, xPieceShift, yPieceShift); // set shader uniform to proper offset
	glUniform4f(pieceColorUni, 0.9f, 0.9f, 0.9f, 1.0f); // set the fiece color to red

	// takes the position and translates it into piece position offset for rendering
	//								Big Box	(sort of)									Small Box (sort of)
	//xPieceShift = board.WIDTH * (((piecePosition / 9) % 3) / 3.0f + (piecePosition % 3) / 9.0f); 
	//yPieceShift = -board.HEIGHT * ((piecePosition / 27) / 3.0f + ((piecePosition % 9) / 3) / 9.0f);

	// draw the grids (board)
	VAOgrid.Bind();
	glDrawArrays(GL_LINES, 0, 8);
	VAOgrid.Unbind();


	while (xBoard.low || xBoard.high || oBoard.low || oBoard.high != 0) { // check to see if all the pieces are rendered
		if (xBoard.low || xBoard.high != 0) { // check for X pieces
			boardCopy = (xBoard.low != 0) ? &xBoard.low : &xBoard.high; // set correct bitboard half
			positionOffset = (xBoard.low != 0) ? 0 : 64; // position offset to account for index not matching position number
			glUniform4f(pieceColorUni, 0.9f, 0.0f, 0.0f, 1.0f); // set the fiece color to red

			while (*boardCopy) {
				piecePosition = positionOffset + ctz64(*boardCopy); // counts the trailing zeros, aka. the position of the lowest significant bit

				// takes the position and translates it into piece position offset for rendering
				//								Big Box										Small Box
				xPieceShift = board.WIDTH * ((((int)(piecePosition / 9)) % 3) / 3.0f + (piecePosition % 3) / 9.0f);
				yPieceShift = -board.HEIGHT * ((int)(((int)(piecePosition / 9)) / 3) / 3.0f + (int)((piecePosition % 9) / 3) / 9.0f);

				// render X piece at position
				glUniform2f(movePositionShiftUni, xPieceShift, yPieceShift); // set shader uniform to proper offset
				VAOx.Bind();
				glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
				VAOx.Unbind();
				(*boardCopy) &= (*boardCopy) - 1; // flips position bit to zero and all others to one and then and's them aka. clears least signifcant bit
			}
		}
		else {
			boardCopy = (oBoard.low != 0) ? &oBoard.low : &oBoard.high;
			positionOffset = (oBoard.low != 0) ? 0 : 64;
			glUniform4f(pieceColorUni, 0.0f, 0.0f, 0.9f, 1.0f); // set the fiece color to red


			while (*boardCopy) {
				piecePosition = positionOffset + ctz64(*boardCopy); // counts the trailing zeros, aka. the position of the lowest significant bit

				// takes the position and translates it into piece position offset for rendering
				//								Big Box										Small Box
				xPieceShift = board.WIDTH * ((((int)(piecePosition / 9)) % 3) / 3.0f + (piecePosition % 3) / 9.0f);
				yPieceShift = -board.HEIGHT * ((int)(((int)(piecePosition / 9)) / 3) / 3.0f + (int)((piecePosition % 9) / 3) / 9.0f);

				// render X piece at position
				glUniform2f(movePositionShiftUni, xPieceShift, yPieceShift); // set shader uniform to proper offset
				VAOo.Bind();
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 82);
				VAOo.Unbind();
				(*boardCopy) &= (*boardCopy) - 1; // flips position bit to zero and all others to one and then and's them aka. clears least signifcant bit
			}

		}

	}



	// check O-board
	// render O's
	// render board
	return;
}

//void Board::renderPiece(VAO VAO, int shapeIndex) {
//	VAO.Bind();
//
//	GLint ebo = 0;
//	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo);
//
//	if (ebo != 0) glDrawElements(GL_TRIANGLES, shapeIndex, GL_UNSIGNED_INT, 0); // draw and X
//	else glDrawArrays(GL_TRIANGLE_STRIP, 0, shapeIndex); // draw and O
//	VAO.Unbind();
//}


static void cursorCallbackFunction(GLFWwindow* window, double xPos, double yPos) {

	Board* board = static_cast<Board*>(glfwGetWindowUserPointer(window));

	board->mouseX = (float)xPos - board->WIDTH / 2.0f;
	board->mouseY = board->HEIGHT / 2.0f - (float)yPos;

	//std::cout << board->mouseX << " , " << board->mouseY << std::endl;
}

static void mouseCallbackFunction(GLFWwindow* window, int button, int action, int mods) {
	if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS) return;

	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	Board* board = static_cast<Board*>(glfwGetWindowUserPointer(window));

	float xPosAdj = (float) xPos - board->WIDTH / 2.0f;
	float yPosAdj = board->HEIGHT / 2.0f - (float) yPos;

	int gridxPos = 9 * xPos / board->WIDTH;
	int gridyPos = 9 * yPos / board->HEIGHT;

	int position = gridxPos + 6 * (int)(gridxPos / 3) + 3 * gridyPos + 18 * (int)(gridyPos/3);
	board->position = position;
	board->MakeMove(position);

	std::cout << gridxPos << " , " << gridyPos << std::endl;
	std::cout << position << std::endl;

}



// Initialize the board and member variables
Board board;

const int SEGMENTS = 40; // Circle segments
const int VERT_COUNT = (SEGMENTS + 1) * 2; // number of vertices; segments + 1 because a line needs 2 points. times 2 because of outer and inner rings
float outerRadius = board.WIDTH / 18.0f;
float innerRadius = board.WIDTH / 36.0f;

GLfloat gridVertices[] = {
	- board.WIDTH / 6.0f, board.HEIGHT / 2.0f, -board.WIDTH / 6.0f, - board.HEIGHT / 2.0f, board.WIDTH / 6.0f, board.HEIGHT / 2.0f, board.WIDTH / 6.0f, -board.HEIGHT / 2.0f,
	-board.WIDTH / 2.0f, board.HEIGHT / 6.0f, board.WIDTH / 2.0f, board.HEIGHT / 6.0f, -board.WIDTH / 2.0f, -board.HEIGHT / 6.0f, board.WIDTH / 2.0f, -board.HEIGHT / 6.0f,
};

// Vertex array for rendering X's
GLfloat xVertices[] = {
	-9 * board.WIDTH / 18.0f , 17 * board.HEIGHT / 36.0f, -17 * board.WIDTH / 36.0f, 9 * board.HEIGHT / 18.0f, -15 * board.WIDTH / 36.0f, 7 * board.HEIGHT / 18.0f, - 7 * board.WIDTH / 18.0f, 15 * board.HEIGHT / 36.0f,
	-9 * board.WIDTH / 18.0f , 15 * board.HEIGHT / 36.0f, -17 * board.WIDTH / 36.0f, 7 * board.HEIGHT / 18.0f, -15 * board.WIDTH / 36.0f, 9 * board.HEIGHT / 18.0f, - 7 * board.WIDTH / 18.0f, 17 * board.HEIGHT / 36.0f
};

GLuint xIndices[] = {
	0, 1, 2, // left half triangle box 1
	1, 2, 3, // right half triangle box 1
	4, 5, 6, // left half triangle box 2
	5, 6, 7  // right half triangle box 2
};



int main () {
	board.xShapeIndex = sizeof(xIndices);

	GLfloat oVertices[VERT_COUNT * 2]; // 2 floats in each vertex (x,y)

	for (int i = 0; i <= SEGMENTS; i++) { // fills the circle array with relevant vertices
		float theta = 2.0f * M_PI * i / SEGMENTS;
		float x = cos(theta);
		float y = sin(theta);

		int index = i * 4;

		// outer vertex
		oVertices[index + 0] = outerRadius * (x + 1) - (board.WIDTH / 2.0f); // take circle coordinate, scale to size, then shift to left edge
		oVertices[index + 1] = outerRadius * (y - 1) + (board.HEIGHT / 2.0f); // take circle coordinate, scale to size, then shift to top edge

		// outer verte
		oVertices[index + 2] = innerRadius * x + outerRadius - (board.WIDTH / 2.0f);
		oVertices[index + 3] = innerRadius * y - outerRadius + (board.HEIGHT / 2.0f);
	}

	board.oShapeIndex = VERT_COUNT;
	// Initialize GLFW
	glfwInit();


	// Tell GLFW what version you are using (we are using Version 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // First "3" of version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Second "3" of version 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Tell GLFW we are using Core profile (vs. compatibility profile that has outdated commands to be back-compatible)

	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE); // use double buffering
	// Set the coordinates to the right coordinate systems

	// Define board dimensions
	double halfWidth = board.WIDTH / 2.0f, halfHeight = board.HEIGHT / 2.0f;



	// create a window object with GLFW window datatype with a size of 800x800 pixels named "OpenGL Tutorial"
	GLFWwindow* window = glfwCreateWindow(board.WIDTH, board.HEIGHT, "Super Tic Tac Toe", NULL, NULL); // datatype* (width, height, name, fullscreen?, not important)

	glfwSetWindowUserPointer(window, &board);
	glfwSetCursorPosCallback(window, cursorCallbackFunction);
	glfwSetMouseButtonCallback(window, mouseCallbackFunction);

	//check to see if the window had an error generating (and if it does close it)
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}
	//tell GL to use the window we created (load the window into the current context; open the window)
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, board.WIDTH, board.HEIGHT);

// Create projection matrix to define window bounds in terms of width and height
glm::mat4 projection = glm::ortho(
	-halfWidth, halfWidth,
	-halfHeight, halfHeight,
	-1.0, 1.0
);

// Apply Matrix?
glm::mat4 view = glm::mat4(1.0f); // object transformation set as identity
glm::mat4 model = glm::mat4(1.0f); // camera transformation set as identity
glm::mat4 MVP = projection * view * model;

// Create shift vector

Shader shaderProgram("projection.vert", "fragmentTest.frag"); //
shaderProgram.Activate();

GLuint MVPuni = glGetUniformLocation(shaderProgram.ID, "MVP");
glUniformMatrix4fv(MVPuni, 1, GL_FALSE, glm::value_ptr(MVP));

GLuint movePositionShiftUni = glGetUniformLocation(shaderProgram.ID, "movePositionShift");
board.movePositionShiftUni = movePositionShiftUni;
glUniform2f(movePositionShiftUni, board.xPieceShift, board.yPieceShift);

GLuint pieceColorUni = glGetUniformLocation(shaderProgram.ID, "pieceColor");
glUniform4f(pieceColorUni, 0.9f, 0.0f, 0.0f, 1.0f);


// Generate Vertex array  for X-piece and bind it
VAO VAOx;
//board.VAOx = &VAOx;
VAOx.Bind();

//Generates VBO and links it to xVertices
VBO VBOx(xVertices, sizeof(xVertices));
//Generates EBO and links it to indices
EBO EBOx(xIndices, sizeof(xIndices));

VAOx.LinkAttrib(VBOx, 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);

VAOx.Unbind();
VBOx.Unbind();
EBOx.Unbind();

// Generate Vertex array for O-piece and bind it
VAO VAOo;
//board.VAOo = &VAOo;
VAOo.Bind();

//Generates VBO and links it to xVertices
VBO VBOo(oVertices, sizeof(oVertices));
////Generates EBO and links it to indices
//EBO EBOo(xIndices, sizeof(xIndices));

VAOo.LinkAttrib(VBOo, 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);

VAOo.Unbind();
VBOo.Unbind();

// Generate Vertex array for board grid and bind it
VAO VAOgrid;
//board.VAOgrid = &VAOgrid;
VAOgrid.Bind();

//Generates VBO and links it to xVertices
VBO VBOgrid(gridVertices, sizeof(gridVertices));
////Generates EBO and links it to indices
//EBO EBOo(xIndices, sizeof(xIndices));

VAOgrid.LinkAttrib(VBOgrid, 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);

VAOgrid.Unbind();
VBOgrid.Unbind();


	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);


	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		//VAOx.Bind();
		//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		//VAOo.Bind();
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, (SEGMENTS + 1) * 2);
		//glUniform2f(movePositionShiftUni, board.xPieceShift, board.yPieceShift);
		glClear(GL_COLOR_BUFFER_BIT);

		renderGameState(board, VAOx, VAOo, VAOgrid, movePositionShiftUni, pieceColorUni);

		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;


}

// Things to implement:
//	Check file names to create new games
//	Access cursor position in the window to track moves made
//		- Create function/nested if statements like legal move function to figure out what/how to render
//	Render board
//	Render pieces
//		-X's
//		-O's