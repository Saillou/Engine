#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../Wrapper/Shader.hpp"
#include "../../Wrapper/Array.hpp"
#include "../../Wrapper/Buffer.hpp"
#include "../../Wrapper/Texture.hpp"

#include <string>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct TextureData {
    std::string type;
    std::unique_ptr<Texture> data;
};

struct Mesh {
    friend struct Model;
    friend struct PrimitiveHelper;

    Mesh();

    void updateBatch(const std::vector<glm::mat4>& models, const std::vector<glm::vec4>& colors = {});

    void bindTextures(Shader& shader) const;
    void unbindTextures() const;

    void drawElements() const;

    const std::vector<Vertex>&       vertices() const;
    const std::vector<unsigned int>& indices()  const;
    const glm::mat4&                 obb()      const;

    std::vector<TextureData>& textures();

private:
    // Gpu data
    Array  m_vao;
    Buffer m_vbo;
    Buffer m_ebo;
    Buffer m_colors;    // TODO: Change for material
    Buffer m_instances;

    // Cpu data
    std::vector<Vertex>       m_vertices = {};
    std::vector<unsigned int> m_indices  = {};
    std::vector<TextureData>  m_textures = {};

    // Simplification
    glm::mat4 m_obb = glm::mat4(1.0f);

    // Bind data Cpu to Gpu
    void _setup();
    void _compute_obb();
};