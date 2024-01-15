#include "Cube.hpp"

#include <vector>
#include <memory>
#include <glm/glm.hpp>

using namespace glm;

std::unique_ptr<Mesh> Cube::CreateMesh() {
	std::unique_ptr<Mesh> cubeMesh = std::make_unique<Mesh>();

    const mat3 world(1.0f);

    // Space dimensions
    const vec3 u = world[0];
    const vec3 v = world[1];
    const vec3 w = world[2];

    // Vertices positions
    const vec3 pA = -u + v - w;
    const vec3 pB = +u + v - w;
    const vec3 pC = +u - v - w;
    const vec3 pD = -u - v - w;

    const vec3 pE = -u + v + w;
    const vec3 pF = +u + v + w;
    const vec3 pG = +u - v + w;
    const vec3 pH = -u - v + w;

    // Texture coordinates
    const vec2 tA = glm::vec2(0.0f, 0.0f);
    const vec2 tB = glm::vec2(1.0f, 0.0f);
    const vec2 tC = glm::vec2(0.0f, 1.0f);
    const vec2 tD = glm::vec2(1.0f, 1.0f);

    // Create triangles
    _createQuad(*cubeMesh, pD, pC, pB, pA, -w, tA, tB, tC, tD);
    _createQuad(*cubeMesh, pH, pG, pF, pE, +w, tA, tB, tC, tD);

    _createQuad(*cubeMesh, pE, pA, pD, pH, -u, tA, tB, tC, tD);
    _createQuad(*cubeMesh, pF, pB, pC, pG, +u, tA, tB, tC, tD);

    _createQuad(*cubeMesh, pD, pC, pG, pH, -v, tA, tB, tC, tD);
    _createQuad(*cubeMesh, pA, pB, pF, pE, +v, tA, tB, tC, tD);

    // Send to gpu
    cubeMesh->_setup();

	return cubeMesh;
}

int Cube::_addPoint(Mesh& mesh, const glm::vec3& vec, const glm::vec3& norm, const glm::vec2& uv)
{
    mesh.m_vertices.push_back(Vertex{ vec, norm, uv});
    return (int)mesh.m_vertices.size()-1;
}

void Cube::_addAsTriangle(Mesh& mesh, unsigned int i0, unsigned int i1, unsigned int i2)
{
    mesh.m_indices.push_back(i0);
    mesh.m_indices.push_back(i1);
    mesh.m_indices.push_back(i2);
}

// Private
void Cube::_createQuad(Mesh& mesh,
    const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3,
    const glm::vec3& n,
    const glm::vec2& T0, const glm::vec2& T1, const glm::vec2& T2, const glm::vec2& T3
)
{
    int i0 = _addPoint(mesh, P0, n, T0);
    int i1 = _addPoint(mesh, P1, n, T1);
    int i2 = _addPoint(mesh, P2, n, T2);
    int i3 = _addPoint(mesh, P3, n, T3);

    _addAsTriangle(mesh, i0, i1, i2);
    _addAsTriangle(mesh, i2, i3, i0);
}