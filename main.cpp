#include "main.hpp"

display::display(const char* title, const int w, const int h) : window(nullptr), glcontext(nullptr), vertexShader(0), fragmentShader(0), shaderProgram(0)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		exception = "ERROR: Unable to initialize SDL ";
		exception += SDL_GetError();
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
		throw *this;
	}

	/*** Create OpenGL context ***/
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	if(glcontext == nullptr)
	{
		exception = "ERROR: Could not create OpenGL context ";
		exception += SDL_GetError();
		throw *this;
	}

	/*** Initialize GLEW ***/
	GLenum err = glewInit();
	if(GLEW_OK != err)
	{
		exception = "ERROR: glewInit failed";
		throw *this;
	}

	/*** Print OpenGL information ***/
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;

	/*** Load shaders ***/
	if(loadShaders("vertex.glsl", "fragment.glsl") != 0)
	{
		throw *this;
	}
}

display::~display()
{
	std::cout << "Cleaning up resources" << std::endl;
	if(shaderProgram != 0) glDeleteProgram(shaderProgram);
	if(fragmentShader != 0) glDeleteShader(fragmentShader);
	if(vertexShader != 0) glDeleteShader(vertexShader);
	if(glcontext != nullptr) SDL_GL_DeleteContext(glcontext);
	if(window != nullptr) SDL_DestroyWindow(window);
	SDL_Quit();
}

void display::update()
{
	SDL_GL_SwapWindow(window);
}

int display::readFile(std::string& fileContent, const char* filePath)
{
	std::fstream file;
	file.open(filePath, std::fstream::in);
	if(!file.is_open())
	{
		exception = "ERROR: Could not open ";
		exception += filePath;
		return 1;
	}

	std::string line;
	while(std::getline(file, line))
	{
		fileContent += line;
		fileContent.push_back('\n');
	}
	file.close();
	return 0;
}

int display::loadShaders(const char* vertexPath, const char* fragmentPath)
{
	/*** Load from file and compile vertex shader ***/
	std::string vertexCode;
	if(readFile(vertexCode, vertexPath) != 0)
	{
		return 1;
	}
	const char* vertexCodeCstr = vertexCode.c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCodeCstr, 0);
	glCompileShader(vertexShader);

	GLint isCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
		std::cout << infoLog << std::endl;

		exception = "ERROR: Failed to compile vertex shader";
		return 1;
	}

	/*** Load from file and compile fragment shader ***/
	std::string fragmentCode;
	if(readFile(fragmentCode, fragmentPath) != 0)
	{
		return 1;
	}
	const char* fragmentCodeCstr = fragmentCode.c_str();

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentCodeCstr, 0);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
		std::cout << infoLog << std::endl;

		exception = "ERROR: Failed to compile fragment shader";
		return 1;
	}

	/*** Link shaders into program ***/
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint isLinked = GL_FALSE;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);
		std::cout << infoLog << std::endl;

		exception = "ERROR: Failed to link shader program";
		return 1;
	}

	glDetachShader(shaderProgram, fragmentShader);
	glDetachShader(shaderProgram, vertexShader);

	return 0;
}

std::ostream& operator<<(std::ostream& out, const std::vector<GLchar>& v)
{
	for(unsigned int i = 0; i < v.size(); i++)
	{
		out << v[i];
	}

	return out;
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

		SDL_Event winEvent;
		bool quit = false;
		while(!quit)
		{
			while(SDL_PollEvent(&winEvent))
			{
				if(winEvent.type == SDL_QUIT) quit = true;
			}

			/*** Drawing ***/
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			disObj.update();
		}
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
