#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

int main() {
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
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}
	//tell GL to use the window we created (introduce the window into the current context)
	glfwMakeContextCurrent(window);

	//open the window (Load GLAD so it configures OpenGL
	gladLoadGL();

	glViewport(0, 0, 800, 800);

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);




	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}