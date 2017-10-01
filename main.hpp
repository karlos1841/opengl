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
	private:
		int readFile(std::string&, const char*);
		GLuint loadShaders(const char*, const char*);
		std::string exception;
		SDL_Window *window;
		SDL_GLContext glcontext;
};

#endif
