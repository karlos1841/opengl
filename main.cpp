#include "main.hpp"
#include "mesh.hpp"

display::display(const char* title, const int w, const int h, const std::vector<const char*>& shaderPaths) : window(nullptr), glcontext(nullptr)
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

	/*** Paths to shaders in vector must be even and one after another ***/
	if(shaderPaths.size() % 2 == 0)
	{
		GLuint shaderProgram = 0;
		for(unsigned int i = 1; i < shaderPaths.size(); i += 2)
		{
			/*** Link the shaders into program ***/
			shaderProgram = createProgram(shaderPaths[i - 1], shaderPaths[i]);
			if(shaderProgram == 0)
			{
				exception = "ERROR: Could not create program";
				throw *this;
			}
			shaderPrograms.push_back(shaderProgram);
		}
	}
	else
	{
		exception = "ERROR: Number of paths to shaders must be even";
		throw *this;
	}
}

display::~display()
{
	std::cout << "Cleaning up resources" << std::endl;
	for(unsigned int i = 0; i < shaderPrograms.size(); i++)
	{
		glDeleteProgram(shaderPrograms[i]);
	}
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
		std::cout << "Could not open " << filePath << std::endl;
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

GLuint display::createProgram(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string vertexShaderCode;
	if(readFile(vertexShaderCode, vertexShaderPath) != 0)
	{
		return 0;
	}
	const GLchar* vertexShaderCodeCstr = vertexShaderCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCodeCstr, 0);
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
		std::cout << "Failed to compile " << vertexShaderPath << std::endl;

		glDeleteShader(vertexShader);
		return 0;
	}

	std::string fragmentShaderCode;
	if(readFile(fragmentShaderCode, fragmentShaderPath) != 0)
	{
		return 0;
	}
	const GLchar* fragmentShaderCodeCstr = fragmentShaderCode.c_str();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCodeCstr, 0);
	glCompileShader(fragmentShader);

	isCompiled = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
		std::cout << infoLog << std::endl;
		std::cout << "Failed to compile " << fragmentShaderPath << std::endl;

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
		return 0;
	}

	/*** Create the program and attach shaders ***/
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	/*** Link the program ***/
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
		std::cout << "Could not link shader program" << std::endl;

		glDeleteProgram(shaderProgram);
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);

		return 0;
	}

	/*** Detach and delete shaders after a successful link ***/
	glDetachShader(shaderProgram, fragmentShader);
	glDetachShader(shaderProgram, vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	/*** Validate the program ***/
	glValidateProgram(shaderProgram);

	GLint isValidated = GL_FALSE;
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &isValidated);
	if(isValidated == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);
		std::cout << infoLog << std::endl;
		std::cout << "Could not validate shader program" << std::endl;

		glDeleteProgram(shaderProgram);
		return 0;
	}

	return shaderProgram;
}

void display::useProgram(unsigned int shaderProgramIndex)
{
	if(shaderProgramIndex < shaderPrograms.size())
	{
		glUseProgram(shaderPrograms[shaderProgramIndex]);
	}
	else
	{
		exception = "ERROR: No such index in useProgram method";
		throw *this;
	}
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
		const std::vector<const char*> shaderPaths = {"vertex.glsl", "fragment.glsl"};
		display disObj("opengl", 800, 600, shaderPaths);
		disObj.useProgram(0);
		mesh meshObj;

		SDL_Event winEvent;
		bool quit = false;
		while(!quit)
		{
			while(SDL_PollEvent(&winEvent))
			{
				if(winEvent.type == SDL_QUIT) quit = true;
			}

			/*** Drawing ***/
			meshObj.draw();

			disObj.update();
		}
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
