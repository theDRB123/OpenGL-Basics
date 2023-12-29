#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
//#define GLEW_STATIC
#include <fstream>
#include <string>
#include <sstream>

//Debugging using glGetError()
//creating an assertion
#define ASSERT(x) if (!(x)) __debugbreak(); 
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__ , __LINE__));
//creating a function to clear the errors
static void GLClearError() {
	while(glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()){
		std::cout << "Error: " << error << " " << function << "\n" << file <<":"<< line<< std::endl;
			
		return false;
	}
	return true;
}

//Debugging using dbug callback 
void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
	__debugbreak();
}



struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {

	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1 , VERTEX = 0 , FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];

	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str()  , ss[1].str() };
}

static unsigned int CompileShader( unsigned int type , const std::string& source ) {
	unsigned int id = glCreateShader(type);
	
	const char* src = source.c_str();//this will return a pointer to the beginning of the string (c_str() returns the pointer to the beginning of the string)
	glShaderSource( id , 1 , &src , nullptr );
	glCompileShader( id );
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id , GL_INFO_LOG_LENGTH , &length );
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}



int main(void)
{
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	GLFWwindow* window;
	
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(600, 600, "This is my triangle", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	int nrAttributes;

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glewInit();

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[] = {
		-0.5f , -0.5f , //index 0
		0.5f , -0.5f , //index 1
		0.5f , 0.5f , //index 2
		-0.5f , 0.5f , //index 3
	};

	unsigned int indices[] = {
		0 , 1 , 2 , //first triangle
		2 , 3 , 0 //second triangle
	};


	//enabling the debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	unsigned int vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	
	unsigned int buffer;
	//this will create a buffer and allot it an id   will be stored in buffer(unsigned int)
	glGenBuffers(1, &buffer);
	//this will bind the buffer to the array buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// filling the buffer with data
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	// will now add the attributes to the buffer
	glEnableVertexAttribArray(0);
	//we only have one buffer (vertex tyoe) so we  will only have to use this once, hence 0
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	//adding the index buffer
	unsigned int ibo;
	//this will create a buffer and allot it an id   will be stored in buffer(unsigned int)
	glGenBuffers(1, &ibo);
	//this will bind the buffer to the array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	// filling the buffer with data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices , GL_STATIC_DRAW);

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	/*std::cout << "Vertex Shader" << std::endl;
	std::cout << source.VertexSource << std::endl; 
	std::cout << "fragment Shader" << std::endl;
	std::cout << source.FragmentSource << std::endl;*/


		
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	// after using the required shader, we will define the uniforms for it
	// as it is required that we have a binded shader before we can define the uniforms

	int location = glGetUniformLocation(shader, "u_color");
	//ASSERT(location != -1);
	glUniform4f(location, 0.0f, 0.0f, 0.0f, 1.0f);

	float red = 0.0f;
	float blue = 0.0f;
	float green = 0.0f;
	float increment = 0.005f;


	// unbinding all the attributes before the loop
	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		//glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		/*glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);*/

		glUniform4f(location, red, 0.5, 0.5, 1.0f);
		if (red> 1.0f) {
			increment = -0.005f;
		}
		else if (red < 0.0f) {
			increment = 0.005f;
		}
		red += increment;
		blue += blue*blue*increment*0.001 + increment;
		green += increment;

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//how can i draw a square?
		
		//note: we need to write GLCall inside GLCall  
		glDrawElements(GL_TRIANGLES, 6 , GL_UNSIGNED_INT , nullptr);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process e-vents */
		glfwPollEvents();
	}

	//glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}