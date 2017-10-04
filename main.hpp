#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>

std::ostream& operator<<(std::ostream&, const std::vector<GLchar>&);

class display : public std::exception
{
	public:
		display(const char*, const int, const int);
		~display();
		const char* what() const noexcept {return exception.c_str();}
		void update();
		void useProgram();
	private:
		int readFile(std::string&, const char*);
		GLuint createShader(GLenum shaderType, const char*); // returns 0 on error
		GLuint createProgram(GLuint* shaders, size_t numOfShaders); // returns 0 on error
		std::string exception;
		SDL_Window *window;
		SDL_GLContext glcontext;
		GLuint shaderProgram;
};

#endif
