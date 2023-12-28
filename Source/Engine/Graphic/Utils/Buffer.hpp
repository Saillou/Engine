#pragma once

#include <vector>
#include "glad/glad.h"

class Buffer {
public:
	explicit Buffer(unsigned int glBufferType, unsigned int glMemoryType = GL_STATIC_DRAW);
	virtual ~Buffer();

	Buffer& operator=(const Buffer&)	= delete;
	Buffer(const Buffer&)				= delete;
	Buffer(Buffer&&)					= delete;

	template <typename T> inline
		void bindData(const std::vector<T>& data);

	void bindData(size_t size);
	void unbind();

	unsigned int getId() const;

protected:
	unsigned int m_bufferId;
	unsigned int m_bufferType;
	unsigned int m_memoryType;
};

template <typename T> inline
	void Buffer::bindData(const std::vector<T>& data) 
{
	glBindBuffer(m_bufferType, m_bufferId);
	glBufferData(m_bufferType, data.size() * sizeof(T), data.data(), m_memoryType);
}