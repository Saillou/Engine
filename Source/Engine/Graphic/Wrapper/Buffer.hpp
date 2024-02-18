#pragma once

#include <vector>
#include "glad/glad.h"

class Buffer {
public:
	Buffer(unsigned int glBufferType, unsigned int glMemoryType = GL_STATIC_DRAW);
	virtual ~Buffer();

	Buffer& operator=(const Buffer&) = delete;
	Buffer(const Buffer&) = delete;
	Buffer(Buffer&&) = delete;

	template <typename T> inline
		void bindData(const std::vector<T>& data);

	template <typename T> inline
		void bindSubData(const std::vector<T>& data);

	void bindData(size_t size);
	void bind() const;
	void unbind() const;

	unsigned int getId() const;
	size_t size() const;

protected:
	unsigned int m_bufferId;
	unsigned int m_bufferType;
	unsigned int m_memoryType;
	size_t		 m_size = 0;
};

template <typename T> inline
	void Buffer::bindData(const std::vector<T>& data) 
{
	m_size = data.size() * sizeof(T);

	bind();
	glBufferData(m_bufferType, m_size, data.data(), m_memoryType);
}

template <typename T> inline
void Buffer::bindSubData(const std::vector<T>& data)
{
	bind();
	glBufferSubData(m_bufferType, 0, data.size() * sizeof(T), data.data());
}