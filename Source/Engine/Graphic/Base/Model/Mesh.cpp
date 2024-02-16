#include "Mesh.hpp"

using namespace glm;

Mesh::Mesh() :
    m_vbo(GL_ARRAY_BUFFER),
    m_ebo(GL_ELEMENT_ARRAY_BUFFER),
    m_colors(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
    m_instances(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW)
{
}

void Mesh::updateBatch(const std::vector<mat4>& models, const std::vector<vec4>& colors) {
    m_instances.bindData(models);

    if (colors.size() >= models.size()) {
        m_colors.bindData(colors);
    }
    else {
        std::vector<vec4> res_colors = colors;
        res_colors.resize(models.size());
        m_colors.bindData(res_colors);
    }
}

// render the mesh
void Mesh::bindTextures(Shader& shader) const {
    for (unsigned int i = 0; i < textures.size(); i++) {
        if (!shader.has(textures[i].type))
            continue;

        Texture::activate(GL_TEXTURE0 + i);
        shader.set(textures[i].type, (int)i);
        textures[i].data->bind();
    }
}

void Mesh::unbindTextures() const {
    for (unsigned int i = 0; i < textures.size(); i++) {
        textures[i].data->unbind();
    }
}

void Mesh::drawElements() const {
    m_vao.bind();
    glDrawElementsInstanced(drawMode, (int)indices.size(), GL_UNSIGNED_INT, 0, (GLsizei)m_instances.size() / sizeof(mat4));
    m_vao.unbind();
}

// Getters
const mat4& Mesh::obb() const {
    return m_obb;
}


// initializes all the buffer objects/arrays
void Mesh::sendToGpu() {
    m_vao.bind();

    m_ebo.bindData(indices);
    m_vbo.bindData(vertices);
    m_colors.bindData(sizeof(vec4));
    m_instances.bindData(sizeof(mat4));

    m_vbo.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    m_colors.bind();
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glVertexAttribDivisor(3, 1);

    m_instances.bind();
    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(4 + i);
        glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(i * sizeof(vec4)));
        glVertexAttribDivisor(4 + i, 1);
    }

    m_vao.unbind();
}

void Mesh::compute_obb() {
    // Get the centroid
    vec3 centroid(0.0f);
    vec3 min_vert = vertices.front().Position;
    vec3 max_vert = vertices.front().Position;

    for (const Vertex& vertex : vertices) {
        centroid += vertex.Position;
        min_vert = min(min_vert, vertex.Position);
        max_vert = max(max_vert, vertex.Position);
    }
    centroid /= (float)(vertices.size());

    // Extends
    vec3 ext_vert = max_vert - min_vert;

    // Final transformation
    m_obb = scale(translate(m_obb, centroid), ext_vert * 0.5f);
}
