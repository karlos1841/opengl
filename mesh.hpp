#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <GL/glew.h>

class mesh
{
	public:
		mesh();
		void draw(const unsigned int);
	private:
		std::vector<GLuint> VAO;
		std::vector<GLuint> VBO;
};

#endif
