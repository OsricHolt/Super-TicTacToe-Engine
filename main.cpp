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


// Generate Vertex array  for X-piece and bind it
VAO VAOx;
board.VAOx = &VAOx;
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
board.VAOo = &VAOo;
VAOo.Bind();

//Generates VBO and links it to xVertices
VBO VBOo(oVertices, sizeof(oVertices));
////Generates EBO and links it to indices
//EBO EBOo(xIndices, sizeof(xIndices));

VAOo.LinkAttrib(VBOo, 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);

VAOo.Unbind();
VBOo.Unbind();

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