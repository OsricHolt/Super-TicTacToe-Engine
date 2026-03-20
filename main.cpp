#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


int main() {

	// Vertex coordinates
	GLfloat vertices[] = {
		//				COORDINATES					  /		COLORS			//
		-0.5f,  -0.5f * float(sqrt(3)) / 3,     0.0f,	0.8f, 0.3f,  0.02f, // bottom left vertex
		 0.5f,  -0.5f * float(sqrt(3)) / 3,     0.0f,	0.8f, 0.3f,  0.02f, // bottom right vertex
		 0.0f,   0.5f * float(sqrt(3)) * 2 / 3, 0.0f,	1.0f, 0.6f,  0.32f, // top middle vertex
		-0.25f,  0.5f * float(sqrt(3)) / 6,     0.0f,	0.9f, 0.45f, 0.17f, // left midpoint vertex
		 0.25f,  0.5f * float(sqrt(3)) / 6,     0.0f,	0.9f, 0.45f, 0.17f, // right midpoint vertex
		 0.0f,  -0.5f * float(sqrt(3)) / 3,     0.0f,	0.8f, 0.3f,  0.02f, // bottom midpoint vertex
	}; // define the vertices of a triangle

	GLuint indices[] = {
		0, 3, 5, // bottom left triangle
		3, 2, 4, // top middle triangle
		5, 4, 1 // bottom right triangle
	};

	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version you are using (we are using Version 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // First "3" of version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Second "3" of version 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Tell GLFW we are using Core profile (vs. compatibility profile that has outdated commands to be back-compatible)


	// create a window object with GLFW window datatype with a size of 800x800 pixels named "OpenGL Tutorial"
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Tutorial", NULL, NULL); // datatype* (width, height, name, fullscreen?, not important)

	//check to see if the window had an error generating (and if it does close it)
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}
	//tell GL to use the window we created (load the window into the current context; open the window)
	glfwMakeContextCurrent(window);

	// Load glad into OpenGL
	gladLoadGL();

	glViewport(0,0, 800,800); // area of OpenGL we want to render in from an (x, y) of (0, 0) to (800, 800)

	// Create shader program with vertex shader and fragment shader as inputs
	Shader shaderProgram("default.vert", "default.frag");

	// Generates VAO object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0); // (VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*) (3 * sizeof(float))); // (VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
	// Unbind everything for safety
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale"); // sets uniID to the address of scale in the shader program


	// Keep the window open unless the close button is pressed or something else closes the window
	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); //Navy Blue; prepare to clear color and replace it (color back buffer)
		glClear(GL_COLOR_BUFFER_BIT); // configure buffer swap for color buffer
		shaderProgram.Activate(); // Run shader program
		glUniform1f(uniID, 0.5f); // sets the scale value to 0.5f in the shader file
		VAO1.Bind(); // Bind vertex array
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0); // Element draw fxn (primitve (shape), # of indices drawn, datatype, indices index)
		glfwSwapBuffers(window); // swap buffer (display triangle!)
		glfwPollEvents(); // process events sent to the window like resizing or clicking or window changes
	}

	//Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// delete the window when we are done with it
	glfwDestroyWindow(window);

	//end OpenGL when we are done with the program
	glfwTerminate();
	return 0;
}