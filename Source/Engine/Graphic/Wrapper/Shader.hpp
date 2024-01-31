#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "ShaderSource.hpp"

struct Shader
{
	friend struct Cookable;

	Shader();
	virtual ~Shader();

	// First, use on of these:
	Shader& attachSource(const unsigned int shader_type, const ShaderSource& source);

	// Then, after attached:
	Shader& link();
	Shader& use();
	static void Release();

	Shader& set(const std::string& name, float v);
	Shader& set(const std::string& name, int v);
	Shader& set(const std::string& name, bool b);

	Shader& set(const std::string& name, const glm::vec3& vec);
	Shader& set(const std::string& name, const glm::vec4& vec);

	Shader& set(const std::string& name, const glm::mat3& mat);
	Shader& set(const std::string& name, const glm::mat4& mat);
	Shader& setBlock(const std::string& name, const int layout);

	bool linked() const;
	bool has(const std::string& name);
	unsigned int id() const;

	bool hasSource(const unsigned int shader_type) const;
	const ShaderSource& source(const unsigned int shader_type) const;

private:
	unsigned int m_id = 0;
	bool m_linked = false;
	std::unordered_map<unsigned int, ShaderSource> m_sources;
};

typedef std::unique_ptr<Shader> UShader;