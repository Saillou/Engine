#include "Mesh.hpp"

using namespace glm;

Mesh::Mesh() :
    m_vbo(GL_ARRAY_BUFFER),
    m_ebo(GL_ELEMENT_ARRAY_BUFFER)
{
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

void Mesh::drawElements(size_t nElements) const {
    m_vao.bind();
    glDrawElementsInstanced(drawMode, (int)indices.size(), GL_UNSIGNED_INT, 0, (GLsizei)nElements);
}

// Getters
const mat4& Mesh::obb() const {
    return m_obb;
}


// initializes all the buffer objects/arrays
void Mesh::setupVao() {
    m_vao.bind();

    m_ebo.bindData(indices);
    m_vbo.bindData(vertices);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    // not unbound, we may want to add stuff on this vao just at higher level (model)
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
