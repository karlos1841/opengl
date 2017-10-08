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
		display(const char*, const int, const int, const std::vector<const char*>&);
		~display();
		const char* what() const noexcept {return exception.c_str();}
		void update();
		void useProgram(unsigned int);
	private:
		int readFile(std::string&, const char*);
		GLuint createProgram(const char*, const char*); // returns 0 on error
		std::string exception;
		SDL_Window *window;
		SDL_GLContext glcontext;
		std::vector<GLuint> shaderPrograms;
};

#endif
