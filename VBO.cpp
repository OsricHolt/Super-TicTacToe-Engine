#include "VBO.h"

// Constructor that generates a VBO and linkis it to vertices
VBO::VBO(GLfloat* vertices, GLsizeiptr size) {
	glGenBuffers(1, &ID); // generate buffers (1 object, address of array of objects)
	glBindBuffer(GL_ARRAY_BUFFER, ID); //GL_ARRAY_BUFFER is vertex buffer type
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

}

// Binds the VBO
void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID); //GL_ARRAY_BUFFER is vertex buffer type
}

// Unbinds the VBO
void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0); //GL_ARRAY_BUFFER is vertex buffer type
}

// Deletes the VBO
void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}