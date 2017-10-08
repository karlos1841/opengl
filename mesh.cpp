#include "mesh.hpp"

mesh::mesh()
{
	/*
	const GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
	};
	*/

	const GLfloat vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
	};

	const GLuint indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
	};

	/*** Generate vertex array object ***/
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	/*** Bind vertex array object ***/
	glBindVertexArray(VAO);

	/*** Generate vertex buffer object ***/
	GLuint VBO;
	glGenBuffers(1, &VBO);

	/*** Bind the buffer to GL_ARRAY_BUFFER and copy data to the buffer ***/
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*** How should we interpret data in the buffer ***/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/*** Specify the index of the vertex attribute ***/
	glEnableVertexAttribArray(0);
}

void mesh::draw()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
