#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <cstdlib>
#include <exception>
#include <SDL2/SDL.h>
#include <GL/glew.h>

class display : public std::exception
{
	public:
		display(const char*, const int, const int);
		~display();
		const char* what() const noexcept {return exception.c_str();}
		void update();
	private:
		void cleanUp();
		std::string exception;
		SDL_Window *window;
		SDL_GLContext glcontext;
};

#endif
