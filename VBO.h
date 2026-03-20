#ifndef _VBO_CLASS_H_
#define _VBO_CLASS_H_

#include <glad/glad.h>

class VBO {
	public:
		// Reference ID for the Vertex Buffer Object
		GLuint ID;
		// Constructor to generate a VBO and link it to its vertices 
		VBO(GLfloat* vertices, GLsizeiptr size);

		// Binds the VBO
		void Bind();
		// Unbinds the VBO
		void Unbind();
		// Deletes the VBO
		void Delete();
};

#endif