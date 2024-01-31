#pragma once

#include "Buffer.hpp"

#include <glad/glad.h>
#include <vector>

template <typename T>
class StorageBuffer : public Buffer {
public:
	// Constructors
	explicit StorageBuffer(unsigned int glMemoryType) : Buffer(GL_SHADER_STORAGE_BUFFER, glMemoryType) { }
	virtual ~StorageBuffer() = default;

	StorageBuffer& operator=(const StorageBuffer&)	= delete;
	StorageBuffer(const StorageBuffer&)				= delete;
	StorageBuffer(StorageBuffer&&)					= delete;

	// Set CPU to GPU
	void bindData(const std::vector<T>& data) {
		m_cpuData = data;
		Buffer::bindData(m_cpuData);
	}

	void bindData(size_t nElements) {
		m_cpuData.resize(nElements, 0.0f);
		Buffer::bindData(nElements * sizeof(T));
	}

	void bindUniform(const int layout) {
		glBindBufferBase(GL_UNIFORM_BUFFER, layout, m_bufferId);
		glBufferSubData(GL_UNIFORM_BUFFER, layout, m_cpuData.size() * sizeof(T), m_cpuData.data());
	}

	// Bind to GPU
	void bind(unsigned int layout) {
		glBindBufferBase(m_bufferType, layout, m_bufferId);
	}

	// Get GPU to CPU
	void retrieve() {
		glGetBufferSubData(m_bufferType, 0, m_cpuData.size() * sizeof(T), m_cpuData.data());
	}

	// CPU access
	T operator[](const size_t indice) {
		if (m_cpuData.size() < indice)
			return 0.0f;
		return m_cpuData[indice];
	}

	const T* data() const {
		return m_cpuData.data();
	}

protected:
	std::vector<T> m_cpuData;
};