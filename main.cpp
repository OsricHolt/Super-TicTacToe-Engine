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


static void mouseCallbackFunction(GLFWwindow* window, int button, int action, int mods) {
	if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS) return;

	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	Board* board = static_cast<Board*>(glfwGetWindowUserPointer(window));



}

// Initialize the board and member variables
Board board;



int main () {

	// Initialize GLFW
	glfwInit();


	// Tell GLFW what version you are using (we are using Version 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // First "3" of version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Second "3" of version 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Tell GLFW we are using Core profile (vs. compatibility profile that has outdated commands to be back-compatible)

	// Set the coordinates to the right coordinate systems

	// Define board dimensions
	double halfWidth = board.WIDTH / 2.0f, halfHeight = board.HEIGHT / 2.0f;

	// Create projection matrix to define window bounds in terms of width and height
	glm::mat4 projection = glm::ortho(
		-halfWidth, halfWidth,
		-halfHeight, halfHeight,
		-1.0, 1.0
	);

	// Apply Matrix?
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 MVP = projection * view * model;

	Shader shaderProgram("projectionMatrix.vert", "defalut.frag"); //
	shaderProgram.Activate();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "MVP");
	glUniformMatrix4fv(uniID, 1, GL_FALSE, glm::value_ptr(MVP));


	// create a window object with GLFW window datatype with a size of 800x800 pixels named "OpenGL Tutorial"
	GLFWwindow* window = glfwCreateWindow(board.WIDTH, board.HEIGHT, "Super Tic Tac Toe", NULL, NULL); // datatype* (width, height, name, fullscreen?, not important)

	glfwSetWindowUserPointer(window, &board);
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

	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);

	board.MakeMove(0);
	board.MakeMove(62);
	board.MakeMove(62);
	board.MakeMove(63);
	board.MakeMove(64);
	board.MakeMove(80);
	board.MakeMove(81);
	board.MakeMove(100);


	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
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