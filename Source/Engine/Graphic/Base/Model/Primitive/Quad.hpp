#pragma once

#include "PrimitiveHelper.hpp"
#include "../Mesh.hpp"
#include "../../../Wrapper/Shader.hpp"

struct Quad :
    private PrimitiveHelper
{
    // Create entity from mesh
    static std::unique_ptr<Mesh> CreateMesh();

    // Create object to be cpu manipulated
    Quad(float x0 = .0f, float y0 = .0f, float width = 1.f, float height = 1.f);
    ~Quad() = default;

    void drawElements() const;

    float x;
    float y;
    float w;
    float h;
    glm::mat4 transform() const;

    // Draw instructions
    bool absolute_dimensions = false;
    int texture_location     = 0;
    glm::vec4 diffuse_color  = {};

    std::unique_ptr<Mesh> mesh;
};