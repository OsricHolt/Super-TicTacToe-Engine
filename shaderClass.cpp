#include "shaderClass.h"

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create vertex shader (object to send vertices to GPU) and get reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); //generate shader object and get its reference value (returns address of shader)
	// attach vertex shader source to vertex shader object
	glShaderSource(vertexShader, 1, &vertexSource, NULL); // (reference value, # of strings for shader, source code, NULL)
	glCompileShader(vertexShader); // compiles source code into machine code so GPU can understand
	compileErrors(vertexShader, "VERTEX"); // checks for compilation errors in the vertex shader

	// Create fragment shader (object to connect vertices to GPU) and get reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //generate fragment shader and get its reference value
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);// (reference value, # of strings for shader, source code, NULL)
	glCompileShader(fragmentShader); // compiles source code into machine code so GPU can understand
	compileErrors(fragmentShader, "FRAGMENT"); // checks for compilation errors in the fragment shader

	//must "wrap up" shaders into a shader program to use them
	ID = glCreateProgram(); //generates shader program (only 1 type of shader program)

	glAttachShader(ID, vertexShader); //attach vertex shader to program
	glAttachShader(ID, fragmentShader); //attach fragment shader to program
	glLinkProgram(ID); // takes program ingredients and "wraps" them up (sorta like compiling program)
	compileErrors(ID, "PROGRAM"); // checks if the program linked properly

	glDeleteShader(vertexShader); // now that shaders are "wrapped up", we clear the space they were using
	glDeleteShader(fragmentShader); // now that shaders are "wrapped up", we clear the space they were using

}

// Activates shader program
void Shader::Activate() {
	glUseProgram(ID); //select shader program
}

// Deletes shader program

void Shader::Delete() {
	glDeleteProgram(ID);
}

// Checks if the different shader have compiled properly
void Shader::compileErrors(unsigned int shader, const char* type) {
	GLint hasCompiled;
	char infoLog[1014];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if(hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << std::endl;
		}
	}
}