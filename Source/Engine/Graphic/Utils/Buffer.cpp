#include "Buffer.hpp"

Buffer::Buffer(unsigned int glBufferType, unsigned int glMemoryType) :
	m_bufferId(0),
	m_bufferType(glBufferType),
	m_memoryType(glMemoryType) 
{
	glGenBuffers(1, &m_bufferId);
}
Buffer::~Buffer() {
	glDeleteBuffers(1, &m_bufferId);
}

void Buffer::bindData(const size_t size) {
	bind();
	glBufferData(m_bufferType, size, nullptr, m_memoryType);
}

void Buffer::bind() {
	glBindBuffer(m_bufferType, m_bufferId);
}

void Buffer::unbind() {
	glBindBuffer(m_bufferType, 0);
}

unsigned int Buffer::getId() const {
	return m_bufferId;
}