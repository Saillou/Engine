#pragma once

#include <memory>
#include <string>
#include <glm/glm.hpp>

#include "ShaderSource.hpp"

class Shader
{
public:
	Shader();
	virtual ~Shader();

	// First, use on of these:
	Shader& attachSource(const unsigned int shader_type, const std::string& shaderCode);
	Shader& attachFile(const unsigned int shader_type, const std::string& shaderPath);

	// Then, after attached:
	Shader& link();
	Shader& use();

	Shader& set(const std::string& name, float v);
	Shader& set(const std::string& name, int v);
	Shader& set(const std::string& name, bool b);

	Shader& set(const std::string& name, const glm::vec3& vec);
	Shader& set(const std::string& name, const glm::vec4& vec);

	Shader& set(const std::string& name, const glm::mat3& mat);
	Shader& set(const std::string& name, const glm::mat4& mat);
	Shader& setBlock(const std::string& name, const int layout);

	bool has(const std::string& name);
	unsigned int id() const;

private:
	static const std::string _readFromFile(const std::string& path);

	unsigned int m_id = 0;
};

typedef std::unique_ptr<Shader> UShader;