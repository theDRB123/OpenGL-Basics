#pragma once
#include <GL/glew.h>
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

#define ASSERT(x) if (!(x)) __debugbreak(); 
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__ , __LINE__));

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

//Debugging using dbug callback 
void GLAPIENTRY 
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};