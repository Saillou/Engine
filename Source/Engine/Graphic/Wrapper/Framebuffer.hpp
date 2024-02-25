#pragma once

#include <glad/glad.h>

#include "../Base/Model/Primitive/Quad.hpp"
#include "Texture.hpp"

class Framebuffer {
public:
	enum Type {
		Unique,
		Multisample,
		Cubemap
	};

	Framebuffer(Type type, unsigned int width = 0, unsigned int height = 0, GLuint m_target = GL_COLOR_ATTACHMENT0);
	virtual ~Framebuffer();

	Framebuffer& operator=(const Framebuffer&)	= delete;
	Framebuffer(const Framebuffer&)				= delete;
	Framebuffer(Framebuffer&&)					= delete;

	static bool CurrIsUsable();
	static void Blit(const Framebuffer& src, Framebuffer& dst);

	unsigned int id() const;
	const Texture& texture() const;

	void resize(unsigned int width, unsigned int height);
	unsigned int width() const;
	unsigned int height() const;
	Type type() const;

	bool isBound() const;
	void bind() const;
	void unbind() const;
	virtual void clear();

protected:
	const Type		m_type;
	const GLuint	m_target;
	const bool		m_useRBO;

	unsigned int	m_framebufferId;
	unsigned int	m_renderbufferId;
	Texture			m_texture_attached;

	// Mutable for semantic reason:
	//   we want bind / unbind to stay constant
	//   to indicate when the content of the buffer is changed or not 
	mutable bool m_bound = false;

private:
	void _createRenderBufferObject(unsigned int width, unsigned int height);
};