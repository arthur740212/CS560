#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, void* offset)
{
	vbo.Bind();
	if (type == GL_INT) 
	{ 
		glVertexAttribIPointer(layout, numComponents, type, stride, offset);
	}
	else
	{
		glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	}
	glEnableVertexAttribArray(layout);

	vbo.Unbind();
}


void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}