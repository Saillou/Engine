#include "Framebuffer.hpp"

Framebuffer::Framebuffer(Type type, unsigned int width, unsigned int height, GLuint target) :
	m_type(type),
	m_framebufferId(0),
	m_renderbufferId(0),
	m_texture_attached(width, height, GL_RGB,
		type == Unique  ? GL_TEXTURE_2D:
		type == Cubemap ? GL_TEXTURE_CUBE_MAP:
		GL_TEXTURE_2D_MULTISAMPLE
	),
	m_target(target),
	m_useRBO(type != Cubemap)
{
	glGenFramebuffers(1, &m_framebufferId);

	// Create depth cube texture
	if (type == Cubemap)
	{
		GLuint component = target == GL_DEPTH_ATTACHMENT ? GL_DEPTH_COMPONENT : GL_RGB;
		m_texture_attached.bind();
		for (unsigned int i = 0; i < 6; ++i) 
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, component, width, height, 0, component, GL_FLOAT, NULL);
		}

	}

	bind();

	// Attach texture to framebuffer
	if (type == Cubemap) {
		glFramebufferTexture(GL_FRAMEBUFFER, target, m_texture_attached.id(), 0);
	}
	else {
		glFramebufferTexture2D(GL_FRAMEBUFFER, target, m_texture_attached.type(), m_texture_attached.id(), 0);
	}

	// Create and link render buffer object
	if (m_useRBO)
	{
		_createRenderBufferObject(width, height);
	}

	unbind();
}
Framebuffer::~Framebuffer() {
	if(m_useRBO)
		glDeleteRenderbuffers(1, &m_renderbufferId);

	glDeleteFramebuffers(1, &m_framebufferId);
}

bool Framebuffer::CurrIsUsable() {
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void Framebuffer::Blit(const Framebuffer& src, Framebuffer& dst) {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, src.id());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst.id());

	glBlitFramebuffer(
		0, 0, src.width(), src.height(), 
		0, 0, dst.width(), dst.height(), 
		GL_COLOR_BUFFER_BIT, GL_NEAREST
	);

	src.unbind();
	dst.unbind();
}

unsigned int Framebuffer::id() const {
	return m_framebufferId;
}

const Texture& Framebuffer::texture() const {
	return m_texture_attached;
}

void Framebuffer::resize(unsigned int width, unsigned int height) {
	m_texture_attached.resize(width, height);
	
	if (m_useRBO) 
	{
		bind();
		_createRenderBufferObject(width, height);
		unbind();
	}
}

unsigned int Framebuffer::width() const {
	return m_texture_attached.width();
}

unsigned int Framebuffer::height() const {
	return m_texture_attached.height();
}

Framebuffer::Type Framebuffer::type() const {
	return m_type;
}

void Framebuffer::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);
}

void Framebuffer::unbind() const {
	m_texture_attached.unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::clear() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::_createRenderBufferObject(unsigned int width, unsigned int height) {
	glDeleteRenderbuffers(1, &m_renderbufferId);

	glGenRenderbuffers(1, &m_renderbufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferId);

	if(m_type == Unique)
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	else
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_DEPTH24_STENCIL8, width, height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbufferId);
}
