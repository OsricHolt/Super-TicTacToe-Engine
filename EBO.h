#ifndef _EBO_CLASS_H_
#define _EBO_CLASS_H_

#include <glad/glad.h>

class EBO {
public:
	// Referencte ID of Elements Buffer Object
	GLuint ID;
	// Constructor that generates a nElements Buffer Object and links it to indices
	EBO(GLuint* indices, GLsizeiptr size);

	// Binds the EBO
	void Bind();
	// Unbinds the EBO
	void Unbind();
	// Deletes the EBO
	void Delete();
};

#endif