#include "PrimitiveHelper.hpp"

using namespace glm;

unsigned int PrimitiveHelper::addPoint(Mesh& mesh, const glm::vec3& vec)
{
    mesh.vertices.push_back(Vertex{ vec, {}, {} });
    return (int)mesh.vertices.size() - 1;
}

unsigned int PrimitiveHelper::addPoint(Mesh& mesh,
    const glm::vec3& vec, const glm::vec3& norm, const glm::vec2& uv)
{
    mesh.vertices.push_back(Vertex{ vec, norm, uv });
    return (int)mesh.vertices.size() - 1;
}

void PrimitiveHelper::addAsTriangle(Mesh& mesh, 
    unsigned int i0, unsigned int i1, unsigned int i2)
{
    mesh.indices.push_back(i0);
    mesh.indices.push_back(i1);
    mesh.indices.push_back(i2);
}

void PrimitiveHelper::createTriangle(Mesh& mesh , const Triangle& triangle)
{
    unsigned int pt0 = addPoint(mesh, triangle[0]);
    unsigned int pt1 = addPoint(mesh, triangle[1]);
    unsigned int pt2 = addPoint(mesh, triangle[2]);

    addAsTriangle(mesh, pt0, pt1, pt2);
}

void PrimitiveHelper::createQuad(Mesh& mesh, const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3)
{
    unsigned int i0 = addPoint(mesh, P0);
    unsigned int i1 = addPoint(mesh, P1);
    unsigned int i2 = addPoint(mesh, P2);
    unsigned int i3 = addPoint(mesh, P3);

    addAsTriangle(mesh, i0, i1, i2);
    addAsTriangle(mesh, i2, i3, i0);
}

void PrimitiveHelper::createQuad(Mesh& mesh,
    const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3,
    const glm::vec3& n,
    const glm::vec2& T0, const glm::vec2& T1, const glm::vec2& T2, const glm::vec2& T3
)
{
    unsigned int i0 = addPoint(mesh, P0, n, T0);
    unsigned int i1 = addPoint(mesh, P1, n, T1);
    unsigned int i2 = addPoint(mesh, P2, n, T2);
    unsigned int i3 = addPoint(mesh, P3, n, T3);

    addAsTriangle(mesh, i0, i1, i2);
    addAsTriangle(mesh, i2, i3, i0);
}

void PrimitiveHelper::setIndices(Mesh& mesh, const std::vector<unsigned int>& idx)
{
    mesh.indices = idx;
}

void PrimitiveHelper::setVertices(Mesh& mesh, const std::vector<float>& vx, const std::vector<float>& nx, const std::vector<float>& ux)
{
    for (size_t i = 0, j = 0; i < vx.size(); i += 3, j += 2) 
    {
        Vertex vertex;
        vertex.Position  = { vx[i + 0], vx[i + 1], vx[i + 2] };
        vertex.Normal    = { nx[i + 0], nx[i + 1], nx[i + 2] };
        vertex.TexCoords = { ux[j + 0], ux[j + 1] };

        mesh.vertices.push_back(vertex);
    }
}
