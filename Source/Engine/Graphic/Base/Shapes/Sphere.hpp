#pragma once

#include <vector>
#include <map>
#include <utility>
#include <memory>
#include <glm/glm.hpp>

#include <memory>
#include "../Model/Mesh.hpp"

struct Sphere {
    static std::unique_ptr<Mesh> CreateMesh(int smoothness = 3);

private:
    static void _generate(int smoothness, float radius);

    static void _addVertex    (float x, float y, float z);
    static void _addNormal    (float nx, float ny, float nz);
    static void _addTexCoord  (float t, float s);
    static void _addIndices   (unsigned int i1, unsigned int i2, unsigned int i3);

    static unsigned int _addSubVertexAttribs(const float v[3], const float n[3], const float t[2]);

    static float _computeScaleForLength(const float v[3], float length);
    static void _computeVertexNormal(const float v[3], float normal[3]);
    static void _computeHalfVertex(const float v1[3], const float v2[3], float length, float newV[3]);
    static void _computeHalfTexCoord(const float t1[2], const float t2[2], float newT[2]);

    static bool _isSharedTexCoord(const float t[2]);
    static bool _isOnLineSegment(const float a[2], const float b[2], const float c[2]);

    static std::map<std::pair<float, float>, unsigned int> _sharedIndices;
    static std::vector<float> m_vertices;
    static std::vector<float> m_normals;
    static std::vector<float> m_uvs;
    static std::vector<unsigned int> m_indices;
};