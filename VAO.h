#ifndef _VAO_CLASS_H_
#define _VAO_CLASS_H_

#include <glad/glad.h>
#include "VBO.h"

class VAO {
public:
	// Reference ID for Vertex Array Object
	GLuint ID;
	// Constructor that generates a VAO ID
	VAO();

	// Links a VBO Attribute to the VAO using a certain layout
	void LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

	// Binds the VAO
	void Bind();
	// Unbinds the VAO
	void Unbind();
	// Deletes the VAO
	void Delete();
};

#endif
