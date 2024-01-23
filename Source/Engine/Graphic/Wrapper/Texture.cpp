#include "Texture.hpp"

#include <iostream>
#include <stb_image.h>

Texture::Texture(GLuint texture_type):
	m_textureId(0),
	m_width(0),
	m_height(0),
	m_texture_type(texture_type)
{
	glGenTextures(1, &m_textureId);
	bind();
	_setParameters();
}

Texture::Texture(const std::string& image_path):
	m_textureId(0),
	m_width(0),
	m_height(0),
	m_texture_type(GL_TEXTURE_2D)
{
	glGenTextures(1, &m_textureId);
	bind();
	load(m_texture_type, image_path, (int*)&m_width, (int*)&m_height);
	_setParameters();
}

Texture::Texture(unsigned int width, unsigned int height, GLuint texture_type) :
	m_textureId(0),
	m_width(width),
	m_height(height),
	m_texture_type(texture_type)
{
	glGenTextures(1, &m_textureId);

	bind();
	resize(m_width, m_height);
	_setParameters();
}

Texture::Texture(GLenum texture_type, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels):
	m_textureId(0),
	m_width(width),
	m_height(height),
	m_texture_type(texture_type)
{
	glGenTextures(1, &m_textureId);
	bind();
	glTexImage2D(
		texture_type, 
		level, internalformat, 
		width, height,
		border, format, type, 
		pixels
	);
	_setParameters();
}

Texture::Texture(const aiTexture* rawTexture): 
	m_textureId(0),
	m_width(0),
	m_height(0),
	m_texture_type(GL_TEXTURE_2D)
{
	glGenTextures(1, &m_textureId);

	stbi_set_flip_vertically_on_load(false);

	int nrComponents;
	unsigned char* data = nullptr;

	if (rawTexture->mHeight == 0)
		data = stbi_load_from_memory((unsigned char*)(rawTexture->pcData), rawTexture->mWidth, (int*)&m_width, (int*)&m_height, &nrComponents, 0);
	else
		data = stbi_load_from_memory((unsigned char*)(rawTexture->pcData), rawTexture->mWidth * rawTexture->mHeight, (int*)&m_width, (int*)&m_height, &nrComponents, 0);
	
	if (!data) {
		std::cerr << "[Warning] Texture failed to read at texture." << std::endl;
		return;
	}

	const GLenum format = nrComponents == 1 ? GL_RED : nrComponents == 4 ? GL_RGBA : GL_RGB;

	bind();
	glTexImage2D(m_texture_type, 0, format, (int)m_width, (int)m_height, 0, format, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	
	_setParameters();
}


Texture::~Texture() {
	glDeleteTextures(1, &m_textureId);
}

unsigned int Texture::id() const {
	return m_textureId;
}

unsigned int Texture::width() const {
	return m_width;
}

unsigned int Texture::height() const {
	return m_height;
}

unsigned int Texture::type() const {
	return m_texture_type;
}

void Texture::bind() const {
	glBindTexture(m_texture_type, m_textureId);
}

void Texture::unbind() const {
	glBindTexture(m_texture_type, 0);
}

void Texture::unbind(GLuint texture_type) {
	glBindTexture(texture_type, 0);
}

void Texture::load(GLuint target, const std::string& path, int* poutWidth, int* poutHeight, int* poutChans) {
	int width; 
	int height; 
	int nrChannels;

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		resize(width, height, data, target);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	// Out info
	if (poutWidth)	*poutWidth	= width;
	if (poutHeight) *poutHeight = height;
	if (poutChans)	*poutChans	= nrChannels;
}

void Texture::resize(unsigned int width, unsigned int height, void* data, GLuint target) {
	m_width  = width;
	m_height = height;

	bind();
	switch (m_texture_type) {
	case GL_TEXTURE_2D_MULTISAMPLE:
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGB, m_width, m_height, GL_TRUE);
		break;

	default:
		glTexImage2D(target == (GLuint)-1 ? m_texture_type : target, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		break;
	}
}

void Texture::activate(GLuint target) {
	glActiveTexture(target);
}

void Texture::deactivate(GLuint texture_type, GLuint target) {
	glActiveTexture(target);
	glBindTexture(texture_type, 0);
}

void Texture::_setParameters() {
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(m_texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_texture_type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}