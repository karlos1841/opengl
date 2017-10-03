#include "mesh.hpp"

glObject::glObject()
{
	const GLfloat diamond[4][2] =
	{{  0.0,  1.0  },
	{  1.0,  0.0  },
	{  0.0, -1.0  },
	{ -1.0,  0.0  }};

	GLuint VAO;
	GLuint VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), diamond, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}
