#pragma once

#include <string>
#include <glad/glad.h>

class Texture {
public:
	Texture(GLuint texture_type = GL_TEXTURE_2D);
	Texture(const std::string& image_path);
	Texture(unsigned int width, unsigned int height, GLuint texture_type = GL_TEXTURE_2D);
	Texture(GLenum texture_type, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);

	virtual ~Texture();

	Texture& operator=(const Texture&)	= delete;
	Texture(const Texture&)				= delete;
	Texture(Texture&&)					= delete;

	unsigned int id() const;
	unsigned int width() const;
	unsigned int height() const;

	void bind() const;
	void unbind() const;
	static void unbind(GLuint texture_type);

	void load(GLuint target, const std::string& path, int* poutWidth = nullptr, int* poutHeight = nullptr, int* poutChans = nullptr);
	void resize(unsigned int width, unsigned int height, void* data = nullptr, GLuint target = -1);

	static void activate(GLuint target);

protected:
	void _setParameters();

	unsigned int m_textureId;
	unsigned int m_width;
	unsigned int m_height;
	GLuint m_texture_type;
};