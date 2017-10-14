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

	const GLfloat vertices1[] = {
	1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.5f, 0.5f, 0.0f
	};
	const GLfloat vertices2[] = {
	-1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f
	};

	const GLuint indices[] = {
	0, 1, 2,
	3, 4, 5
	};

	/*** Generate vertex array objects ***/
	VAO.resize(2);
	glGenVertexArrays(2, &VAO[0]);

	/*** Generate vertex buffer object ***/
	VBO.resize(2);
	glGenBuffers(2, &VBO[0]);

	/*** Generate element buffer object ***/
	GLuint EBO;
	glGenBuffers(1, &EBO);

	/*** Bind vertex array object ***/
	glBindVertexArray(VAO[0]);

	/*** Bind vertex buffer object ***/
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	/*** Copy vertex attributes to the buffer ***/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	/*** Bind element buffer object ***/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	/*** Copy vertex array indices to the buffer ***/
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*** How should we interpret data in the buffer ***/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/*** Specify the index of the vertex attribute ***/
	glEnableVertexAttribArray(0);



	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void mesh::draw(const unsigned int VAOindex)
{
	glBindVertexArray(VAO[VAOindex]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
