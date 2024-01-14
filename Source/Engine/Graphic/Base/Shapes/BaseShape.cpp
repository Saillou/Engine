#include "BaseShape.hpp"
#include "glad/glad.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

BaseShape::BaseShape() : 
    m_vbo_vertices(GL_ARRAY_BUFFER), 
    m_vbo_normals(GL_ARRAY_BUFFER),
    m_vbo_uvs(GL_ARRAY_BUFFER),
    m_ebo(GL_ELEMENT_ARRAY_BUFFER),
    m_colors(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
    m_instances(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
    m_current_attrib_id(0)
{
    // ..
}

void BaseShape::_bindArray() {
    bind();

    // Required: vertices positions
    m_vbo_vertices.bindData(m_vertices);
    glVertexAttribPointer(m_current_attrib_id, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(m_current_attrib_id);
    m_current_attrib_id++;

    // Optional: normals
    if (!m_normals.empty()) {
        m_vbo_normals.bindData(m_normals);
        glVertexAttribPointer(m_current_attrib_id, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(m_current_attrib_id);
        m_current_attrib_id++;
    }

    // Optional: texture coordinates
    if (!m_uvs.empty()) {
        m_vbo_uvs.bindData(m_uvs);
        glVertexAttribPointer(m_current_attrib_id, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
        glEnableVertexAttribArray(m_current_attrib_id);
        m_current_attrib_id++;
    }

    m_ebo.bindData(m_indices);

    m_vbo_vertices.unbind();
    m_vbo_normals.unbind();

    unbind();
}

void BaseShape::createBatch(const std::vector<glm::vec4>& colors, const std::vector<glm::mat4>& models) {
    bind();

    m_colors.bindData(colors);
    glVertexAttribPointer(m_current_attrib_id, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(m_current_attrib_id);
    glVertexAttribDivisor(m_current_attrib_id, 1);
    m_current_attrib_id++;

    m_instances.bindData(models);
    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(m_current_attrib_id);
        glVertexAttribPointer(m_current_attrib_id, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
        glVertexAttribDivisor(m_current_attrib_id, 1);

        m_current_attrib_id++;
    }

    unbind();
}

void BaseShape::updateBatch(const std::vector<glm::vec4>& colors, const std::vector<glm::mat4>& models) {
    m_colors.bindData(colors);
    m_instances.bindData(models);
}

void BaseShape::bind() {
    m_vao.bind();
}

void BaseShape::unbind() {
    m_vao.unbind();
}

int BaseShape::indicesLength() const {
    return (int)m_indices.size();
}

int BaseShape::verticesLength() const {
    return (int)m_vertices.size();
}

int BaseShape::normalsLength() const {
    return (int)m_normals.size();
}

int BaseShape::uvsLength() const {
    return (int)m_uvs.size();
}

const std::vector<unsigned int>& BaseShape::indices() const {
    return m_indices;
}

const std::vector<float>& BaseShape::vertices() const {
    return m_vertices;
}

// Helpers
int BaseShape::_addPoint(float x, float y, float z) {
    m_vertices.push_back(x);
    m_vertices.push_back(y);
    m_vertices.push_back(z);

    return verticesLength() / 3 - 1;
}

int BaseShape::_addPoint(float x, float y, float z, float nx, float ny, float nz) {
    m_normals.push_back(nx);
    m_normals.push_back(ny);
    m_normals.push_back(nz);

    return _addPoint(x, y, z);
}

int BaseShape::_addPoint(float x, float y, float z, float nx, float ny, float nz, float tu, float tv) {
    m_uvs.push_back(tu);
    m_uvs.push_back(tv);

    return _addPoint(x, y, z, nx, ny, nz);
}

int BaseShape::_addPoint(const glm::vec3& vec) {
    return _addPoint(vec.x, vec.y, vec.z);
}

int BaseShape::_addPoint(const glm::vec3& vec, const glm::vec3& norm) {
    return _addPoint(
        vec.x,  vec.y,  vec.z, 
        norm.x, norm.y, norm.z
    );
}

int BaseShape::_addPoint(const glm::vec3& vec, const glm::vec3& norm, const glm::vec2& uv) {
    return _addPoint(
        vec.x,  vec.y,  vec.z,
        norm.x, norm.y, norm.z,
        uv.x,   uv.y
    );
}


void BaseShape::_addAsTriangle(unsigned int i0, unsigned int i1, unsigned int i2) {
    m_indices.push_back(i0);
    m_indices.push_back(i1);
    m_indices.push_back(i2);
}


void BaseShape::_createQuad(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3) {
    int i0 = _addPoint(P0);
    int i1 = _addPoint(P1);
    int i2 = _addPoint(P2);
    int i3 = _addPoint(P3);

    _addAsTriangle(i0, i1, i2);
    _addAsTriangle(i2, i3, i0);
};

void BaseShape::_createQuad(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3, const glm::vec3& n) {
    int i0 = _addPoint(P0, n);
    int i1 = _addPoint(P1, n);
    int i2 = _addPoint(P2, n);
    int i3 = _addPoint(P3, n);

    _addAsTriangle(i0, i1, i2);
    _addAsTriangle(i2, i3, i0);
};

void BaseShape::_createQuad(
    const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3,
    const glm::vec3& n,
    const glm::vec2& T0, const glm::vec2& T1, const glm::vec2& T2, const glm::vec2& T3
) 
{
    int i0 = _addPoint(P0, n, T0);
    int i1 = _addPoint(P1, n, T1);
    int i2 = _addPoint(P2, n, T2);
    int i3 = _addPoint(P3, n, T3);

    _addAsTriangle(i0, i1, i2);
    _addAsTriangle(i2, i3, i0);
}