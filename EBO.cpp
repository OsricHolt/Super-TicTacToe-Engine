#include "EBO.h"

// Constructor generating Element Buffer Object and linking it to indices
EBO::EBO(GLuint* indices, GLsizeiptr size) {
	glGenBuffers(1, &ID); // generate buffers (1 object, address of array of objects)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); //GL_ARRAY_BUFFER is vertex buffer type
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);

}

// Binds the EBO
void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); //GL_ARRAY_BUFFER is vertex buffer type
}

// Unbinds the EBO
void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //GL_ARRAY_BUFFER is vertex buffer type
}

// Deletes the EBO
void EBO::Delete() {
	glDeleteBuffers(1, &ID);
}