#pragma once

#include <string>
#include <glad/glad.h>
#include <assimp/scene.h>
#include <unordered_map>

class Texture {
// Static
public:
	static struct Cache {
		~Cache();

		struct Metadata {
			std::string path;
			int width;
			int height;
			int channels;
			void* pixels;
		};

		static bool Has(const std::string& path);
		static const Metadata& Get(const std::string& path);
		static void Set(const std::string& path, const Metadata&);
		static bool Create(const std::string& path);

		static void Clean();

	private:
		std::unordered_map<std::string, Metadata> _data;
	} s_cache;

// Instance
public:
	Texture(GLenum m_texture_format = GL_RGB, GLuint texture_type = GL_TEXTURE_2D);
	Texture(const std::string& image_path);
	Texture(unsigned int width, unsigned int height, GLenum texture_format, GLuint texture_type);
	Texture(GLenum texture_type, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum texture_format, GLenum type, const void* pixels);
	Texture(const aiTexture* rawTextureData);
	Texture(const Texture&);

	virtual ~Texture();

	Texture& operator=(const Texture&)	= delete;
	Texture(Texture&&)					= delete;

	unsigned int id() const;
	unsigned int width() const;
	unsigned int height() const;
	unsigned int type() const;

	void bind() const;
	void unbind() const;
	static void unbind(GLuint texture_type);

	void CopyTo(Texture&) const;
	void load(GLuint target, const std::string& path, int* poutWidth = nullptr, int* poutHeight = nullptr, int* poutChans = nullptr);
	void resize(unsigned int width, unsigned int height, void* data = nullptr, GLuint target = -1);

	static void activate(GLuint target);
	static void deactivate(GLuint texture_type, GLuint target);

protected:
	void _setParameters();
	static GLenum _GetFormatFromChannel(int nChannels);

	unsigned int m_textureId;
	unsigned int m_width;
	unsigned int m_height;
	GLenum m_texture_format; 
	GLuint m_texture_type; 
};