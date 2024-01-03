#include "IndexBuffer.h"
#include "Renderer.h"


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) 
{
	//this will create a buffer and allot it an id   will be stored in buffer(unsigned int)
	glGenBuffers(1, &m_RendererID);
	//this will bind the buffer to the array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	// filling the buffer with data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
