#include "Shader.hpp"
#include "glad/glad.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

Shader::Shader() : m_id(glCreateProgram()) {

}
Shader::~Shader() {
    glDeleteProgram(m_id);
}

Shader& Shader::attachSource(const unsigned int shader_type, const std::string& shaderCode) {
    const char* psource = shaderCode.c_str();

    // Create shader
    unsigned int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &psource, nullptr);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    // Errors
    char infoLog[512];
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error: \n" << infoLog << std::endl;
    } assert(success && "Fail to compile shader");

    glAttachShader(m_id, shader);

    // Cleanup
    glDeleteShader(shader);
    return *this;
}

Shader& Shader::attachFile(const unsigned int shader_type, const std::string& shaderPath) {
    return attachSource(shader_type, _readFromFile(shaderPath));
}

Shader& Shader::link() {
    glLinkProgram(m_id);

    // Errors
    int success;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);

    char infoLog[512];
    if (!success) {
        glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
        std::cerr << "Error: \n" << infoLog << std::endl;
    } assert(success && "Fail to link shaders");

    return *this;
}
Shader& Shader::use() {
    glUseProgram(m_id);

    return *this;
}

bool Shader::has(const std::string& name) {
    return glGetUniformLocation(m_id, name.c_str()) != -1;
}

unsigned int Shader::id() const {
    return m_id;
}

Shader& Shader::set(const std::string& name, float v) {
    int varLoc = glGetUniformLocation(m_id, name.c_str());
    if (varLoc == -1)
        std::cerr << "[warning] Uniform location not found for `" << name << "`" << std::endl;
    glUniform1f(varLoc, v);

    return *this;
}

Shader& Shader::set(const std::string& name, int v) {
    int varLoc = glGetUniformLocation(m_id, name.c_str());
    if (varLoc == -1)
        std::cerr << "[warning] Uniform location not found for `" << name << "`" << std::endl;
    glUniform1i(varLoc, v);

    return *this;
}

Shader& Shader::set(const std::string& name, bool b) {
    return set(name, (int)b);
}

Shader& Shader::set(const std::string& name, const glm::vec3& vec) {
    int varLoc = glGetUniformLocation(m_id, name.c_str());
    if (varLoc == -1)
        std::cerr << "[warning] Uniform location not found for `" << name << "`" << std::endl;
    glUniform3f(varLoc, vec.x, vec.y, vec.z);

    return *this;
}

Shader& Shader::set(const std::string& name, const glm::vec4& vec) {
    int varLoc = glGetUniformLocation(m_id, name.c_str());
    if (varLoc == -1)
        std::cerr << "[warning] Uniform location not found for `" << name << "`" << std::endl;
    glUniform4f(varLoc, vec.x, vec.y, vec.z, vec.w);

    return *this;
}

Shader& Shader::set(const std::string& name, const glm::mat3& mat) {
    int varLoc = glGetUniformLocation(m_id, name.c_str());
    if (varLoc == -1)
        std::cerr << "[warning] Uniform location not found for `" << name << "`" << std::endl;

    glUniformMatrix3fv(varLoc, 1, GL_FALSE, &mat[0][0]);

    return *this;
}

Shader& Shader::set(const std::string& name, const glm::mat4& mat) {
    int varLoc = glGetUniformLocation(m_id, name.c_str());
    if (varLoc == -1)
        std::cerr << "[warning] Uniform location not found for `" << name << "`" << std::endl;
    glUniformMatrix4fv(varLoc, 1, GL_FALSE, &mat[0][0]);

    return *this;
}

Shader& Shader::setBlock(const std::string& name, const int layout) {
    int blockIndex = glGetUniformBlockIndex(m_id, name.c_str());
    if (blockIndex == -1)
        std::cerr << "[warning] Uniform location not found for `" << name << "`" << std::endl;
    glUniformBlockBinding(m_id, blockIndex, layout);

    return *this;
}

const std::string Shader::_readFromFile(const std::string& path) {
    std::ifstream file(path);
    assert(file.is_open());

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}
