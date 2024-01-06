#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
//#define GLEW_STATIC
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"


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

	//int nrAttributes;

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
	
	
	VertexArray va;
	VertexBufferLayout layout;
	VertexBuffer vb(positions, 4 * 2 * sizeof(float));
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 6);
	
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");



		
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	// after using the required shader, we will define the uniforms for it
	// as it is required that we have a binded shader before we can define the uniforms

	int location = glGetUniformLocation(shader, "u_shift");
	//ASSERT(location != -1);
	//glUniform4f(location, 0.0f, 0.0f, 0.0f, 1.0f);

	float shift_val = 0.0f;
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
		//glBindVertexArray(vao);
		va.Bind();
		ib.Bind();
		/*glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);*/

		glUniform1f(location, shift_val);
		if (shift_val > 0.5f) {
			increment = -0.005f;
		}
		else if (shift_val < -0.5f) {
			increment = 0.005f;
		}
		shift_val += increment;

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