#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	//this will create a buffer and allot it an id   will be stored in buffer(unsigned int)
	glGenBuffers(1, &m_RendererID);
	//this will bind the buffer to the array buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	// filling the buffer with data
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
