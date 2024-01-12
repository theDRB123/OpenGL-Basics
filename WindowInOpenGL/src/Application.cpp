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
#include "Shader.h"
#include "Texture.h"


int main(void)
{
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(800, 800, "This was a triangle", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}


	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glewInit();

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[] = {
		-1.0f , -1.0f , 0.0f , 0.0f ,//index 0
		1.0f , -1.0f , 1.0f , 0.0f, //index 1
		1.0f , 1.0f , 1.0f , 1.0f, //index 2
		-1.0f , 1.0f , 0.0f , 1.0f //index 3
		//-1.0f , -1.0f ,//index 0
		//1.0f , -1.0f , //index 1
		//1.0f , 1.0f , //index 2
		//-1.0f , 1.0f //index 3
	};

	unsigned int indices[] = {
		0 , 1 , 2 , //first triangle
		2 , 3 , 0 //second triangle
	};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC2_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	
	VertexArray va;
	VertexBufferLayout layout;
	VertexBuffer vb(positions, 4 * 4 * sizeof(float));
	layout.Push<float>(2);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 6);
	Shader shader("res/shaders/Basic.shader");
	shader.Bind();

	Texture texture("res/textures/duck.png");
	texture.Bind();

	shader.SetUniform1i("u_Texture",0);

	float shift_val = 0.0f;
	float increment = 0.005f;
	//float increment = 0.005f;


	//glBindVertexArray(0);
	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();
	
	Renderer renderer;


	while (!glfwWindowShouldClose(window))
	{
		renderer.Clear();
		renderer.Draw(va, ib, shader);
		shader.SetUniform1f("u_shift", shift_val);
		/*if (shift_val > 10.0f) {
			increment = -0.005f;
		}
		else if (shift_val < 0.0f) {
			increment = 0.005f;
		}*//*
		increment += increment * 0.01;
		increment += 0.005f;*/
		shift_val += increment;
		std::cout << increment << std::endl;
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}