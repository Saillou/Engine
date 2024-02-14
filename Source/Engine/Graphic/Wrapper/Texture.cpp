#include "Texture.hpp"

#include <iostream>
#include <stb_image.h>

// Statics
Texture::Cache Texture::s_cache;

Texture::Cache::~Cache() {
	Clean();
}

bool  Texture::Cache::Has(const std::string& path) {
	return s_cache._data.find(path) != s_cache._data.cend();
}
const Texture::Cache::Metadata& Texture::Cache::Get(const std::string& path) {
	return s_cache._data.at(path);
}
void  Texture::Cache::Set(const std::string& path, const Metadata& data) {
	s_cache._data[path] = data;
}
bool  Texture::Cache::Create(const std::string& path) {
	if (Has(path))
		return true;

	Metadata metadata;

	metadata.pixels = stbi_load(path.c_str(), &metadata.width, &metadata.height, &metadata.channels, 0);
	if (!metadata.pixels) 	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(metadata.pixels);

		return false;
	}

	Set(path, metadata);
	return true;
}
void Texture::Cache::Clean() {
	for (auto& it : s_cache._data) {
		stbi_image_free(it.second.pixels);
	}
	s_cache._data = {};
}

// Instance
Texture::Texture(GLenum m_texture_format, GLuint texture_type):
	m_textureId(0),
	m_width(0),
	m_height(0),
	m_texture_format(m_texture_format),
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
	m_texture_format(0),
	m_texture_type(GL_TEXTURE_2D)
{
	glGenTextures(1, &m_textureId);
	bind();
	load(m_texture_type, image_path, (int*)&m_width, (int*)&m_height);
	_setParameters();
}

Texture::Texture(unsigned int width, unsigned int height, GLenum m_texture_format, GLuint texture_type) :
	m_textureId(0),
	m_width(width),
	m_height(height),
	m_texture_format(m_texture_format),
	m_texture_type(texture_type)
{
	glGenTextures(1, &m_textureId);

	resize(m_width, m_height);
	_setParameters();
}

Texture::Texture(GLenum texture_type, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels):
	m_textureId(0),
	m_width(width),
	m_height(height),
	m_texture_format(format),
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
	m_texture_format(0),
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

	m_texture_format = _GetFormatFromChannel(nrComponents);

	bind();
	glTexImage2D(m_texture_type, 0, m_texture_format, (int)m_width, (int)m_height, 0, m_texture_format, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	
	_setParameters();
}

Texture::Texture(const Texture& inTexture) :
	m_textureId(0),
	m_width(inTexture.m_width),
	m_height(inTexture.m_height),
	m_texture_format(inTexture.m_texture_format),
	m_texture_type(inTexture.m_texture_type)
{
	glGenTextures(1, &m_textureId);
	bind();
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
	if (!Cache::Create(path))
		return;

	const Cache::Metadata& metadata = Cache::Get(path);

	m_texture_format = _GetFormatFromChannel(metadata.channels);
	resize(metadata.width, metadata.height, metadata.pixels, target);

	// Out info
	if (poutWidth)	*poutWidth	= metadata.width;
	if (poutHeight) *poutHeight = metadata.height;
	if (poutChans)	*poutChans	= metadata.channels;
}

void Texture::resize(unsigned int width, unsigned int height, void* data, GLuint target) {
	m_width  = width;
	m_height = height;

	bind();
	switch (m_texture_type) {
	case GL_TEXTURE_2D_MULTISAMPLE:
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, m_texture_format, m_width, m_height, GL_TRUE);
		break;

	default:
		glTexImage2D(target == (GLuint)-1 ? m_texture_type : target, 0, m_texture_format, m_width, m_height, 0, m_texture_format, GL_UNSIGNED_BYTE, data);
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

GLenum Texture::_GetFormatFromChannel(int nChannels) {
	switch (nChannels)
	{
		case 1: return GL_RED;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
	}

	std::cerr << "Warning texture: channels number incorrect, RGB taken as default | n = " << (nChannels) << std::endl;
	return GL_RGB;
}
