#include "Mesh.hpp"

Mesh::Mesh():
    m_vbo(GL_ARRAY_BUFFER),
    m_ebo(GL_ELEMENT_ARRAY_BUFFER), 
    m_colors(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
    m_instances(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW)
{
}

void Mesh::updateBatch(const std::vector<glm::vec4>& colors, const std::vector<glm::mat4>& models) {
    m_colors.bindData(colors);
    m_instances.bindData(models);
}

// render the mesh
void Mesh::bindTextures(Shader& shader) const {
    for (unsigned int i = 0; i < m_textures.size(); i++) {
        if(!shader.has(m_textures[i].type))
            continue;

        Texture::activate(GL_TEXTURE0 + i);
        shader.set(m_textures[i].type, (int)i);
        m_textures[i].data->bind();
    }
}

void Mesh::unbindTextures() const {
    for (unsigned int i = 0; i < m_textures.size(); i++) {
        m_textures[i].data->unbind();
    }
}

void Mesh::drawElements() const {
    m_vao.bind();

    if(m_instances.size() == 0)
        glDrawElements(GL_TRIANGLES, (unsigned int)m_indices.size(), GL_UNSIGNED_INT, 0);
    else
        glDrawElementsInstanced(GL_TRIANGLES, (int)m_indices.size(), GL_UNSIGNED_INT, 0, (GLsizei)m_instances.size() / sizeof(glm::mat4));

    m_vao.unbind();
}

void Mesh::drawElements(Shader& shader, const glm::mat4& quat) const {
    if(shader.has("LocalModel"))
        shader.use().set("LocalModel", quat);

    drawElements();
}

// Getters
const std::vector<Vertex>& Mesh::vertices() const {
    return m_vertices;
}

const std::vector<unsigned int>& Mesh::indices() const {
    return m_indices;
}

// initializes all the buffer objects/arrays
void Mesh::_setup() {
    m_vao.bind();

    m_ebo.bindData(m_indices);
    m_vbo.bindData(m_vertices);
    m_colors.bindData(sizeof(glm::vec4));
    m_instances.bindData(sizeof(glm::mat4));

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
        glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
        glVertexAttribDivisor(4 + i, 1);
    }

    m_vao.unbind();
}
