#include "Array.hpp"

#include "glad/glad.h"

Array::Array() {
	glGenVertexArrays(1, &m_array_id);
}
Array::~Array() {
	glDeleteVertexArrays(1, &m_array_id);
}

void Array::bind() const {
	glBindVertexArray(m_array_id);
}

void Array::unbind() const {
	glBindVertexArray(0);
}

unsigned int Array::getId() const {
	return m_array_id;
}