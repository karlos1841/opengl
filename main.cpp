#include "main.hpp"

display::display(const char* title, const int w, const int h) : window(nullptr), glcontext(nullptr)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		exception = "ERROR: Unable to initialize SDL ";
		exception += SDL_GetError();
		cleanUp();
		throw *this;
	}

	/*** Print SDL version after initialization ***/
	SDL_version compiled;
	SDL_version linked;
	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	std::cout << "Compiled against SDL version " << std::to_string(compiled.major) << '.' << std::to_string(compiled.minor) << '.' << std::to_string(compiled.patch) << std::endl;
	std::cout << "Linked against SDL version " << std::to_string(linked.major) << '.' << std::to_string(linked.minor) << '.' << std::to_string(linked.patch) << std::endl;

	/*** Set OpenGL attributes before window creation ***/
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	/*** Create window ***/
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL);
	if(window == nullptr)
	{
		exception = "ERROR: Could not create window ";
		exception += SDL_GetError();
		cleanUp();
		throw *this;
	}

	/*** Create OpenGL context ***/
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	if(glcontext == nullptr)
	{
		exception = "ERROR: Could not create OpenGL context ";
		exception += SDL_GetError();
		cleanUp();
		throw *this;
	}

	/*** Initialize GLEW ***/
	GLenum err = glewInit();
	if(GLEW_OK != err)
	{
		exception = "ERROR: glewInit failed";
		cleanUp();
		throw *this;
	}
}

display::~display()
{
	cleanUp();
}

void display::cleanUp()
{
	if(glcontext != nullptr) SDL_GL_DeleteContext(glcontext);
	if(window != nullptr) SDL_DestroyWindow(window);
	SDL_Quit();
}

void display::update()
{
	SDL_GL_SwapWindow(window);
}






int main(int argc, char *argv[])
{
	std::string name;
	for(int i = 0; i < argc; i++)
	{
		name += argv[i];
		name += ' ';
	}
	std::cout << "Invoking " << name << std::endl;


	try
	{
		display disObj("opengl", 800, 600);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		disObj.update();
		SDL_Delay(3000);
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
