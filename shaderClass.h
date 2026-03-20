#ifndef _SHADER_CLASS_H_
#define _SHADER_CLASS_H_

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader {
	public:
		// Reference ID of the shader program
		GLuint ID;
		// Constructor to build the shader from vertex and fragment shader programs
		Shader(const char* vertexFile, const char* fragmentFile);

		// Activates the shader program
		void Activate();
		// Deletes the shader program
		void Delete();
	private:
		// Checks if the different shader have compiled properly
		void compileErrors(unsigned int shader, const char* type);
};


#endif
