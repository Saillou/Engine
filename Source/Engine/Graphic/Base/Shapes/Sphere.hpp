#pragma once

#include <vector>
#include <map>
#include <utility>
#include <memory>
#include <glm/glm.hpp>

#include "BaseShape.hpp"
#include "../../Cookable.hpp"
#include "../../Light.hpp"
#include "../../Camera.hpp"

// Objects
struct Sphere : 
    public BaseShape,
    public Cookable
{
    // Instance
    Sphere(float radius, int smoothness = 3);
    virtual ~Sphere() = default;

    void draw(const Camera& camera, const glm::mat4& quad, const std::vector<Light>& lights = {});
    void draw(const Camera& camera, const glm::vec3& position = {}, const glm::vec3& orientation = {}, const std::vector<Light>& lights = {});
    void drawBatch(size_t amount, const Camera& camera);
    void drawElements(Shader& sh);

private:
    void _addVertex    (float x, float y, float z);
    void _addNormal    (float nx, float ny, float nz);
    void _addTexCoord  (float t, float s);
    void _addIndices   (unsigned int i1, unsigned int i2, unsigned int i3);

    unsigned int _addSubVertexAttribs(const float v[3], const float n[3], const float t[2]);

    static float _computeScaleForLength(const float v[3], float length);
    static void _computeVertexNormal(const float v[3], float normal[3]);
    static void _computeHalfVertex(const float v1[3], const float v2[3], float length, float newV[3]);
    static void _computeHalfTexCoord(const float t1[2], const float t2[2], float newT[2]);

    static bool _isSharedTexCoord(const float t[2]);
    static bool _isOnLineSegment(const float a[2], const float b[2], const float c[2]);

    std::map<std::pair<float, float>, unsigned int> _sharedIndices;
};