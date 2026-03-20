#include "VAO.h"

// Constructor that generates a VAO ID
VAO::VAO() {
	glGenVertexArrays(1, &ID); //MUST generate VAO BEFORE VBO
}

// Links a VBO Attribute to the VAO using a certain layout
void VAO::LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	//how to read VBO (vertex attribute index, values per vertex, value type, interger coordinates?, vertex stride(how much data between verteces, offset)
	glEnableVertexAttribArray(layout); //enable array (position of vertex attribute)
	VBO.Unbind();
}

// Binds the VAO
void VAO::Bind() {
	glBindVertexArray(ID); //select vertex array for VBO
}

// Unbinds the VAO
void VAO::Unbind() {
	glBindVertexArray(0); //clear bind to avoid accidents
}

// Deletes the VAO
void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);

}