#include "VertexBufferLayout.h"
#include <vector>
#include <GL/glew.h>

struct VertexBufferElement {
	
};

VertexBufferLayout::VertexBufferLayout()
{

}

template<typename T>
void VertexBufferLayout::Push(int count)
{
	static_assert(false);
}

template<>
void VertexBufferLayout::Push<float>(int count)
{
	m_Elements.push_back({ GL_FLOAT,count,GL_FALSE });
	m_Stride += sizeof(GLfloat);
}
