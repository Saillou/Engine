#pragma once

#include "PrimitiveHelper.hpp"
#include "../Material.hpp"
#include "../Mesh.hpp"
#include "../../../Wrapper/Shader.hpp"

struct Quad :
    private PrimitiveHelper
{
    Quad(float x0 = .0f, float y0 = .0f, float width = 1.f, float height = 1.f);
    ~Quad() = default;

    void drawElements() const;

    float x() const;
    float y() const;
    float w() const;
    float h() const;

    // Draw instructions
    int texture_location = 0;
    Material material    = {};

private:
    float _x, _y, _w, _h;
    Mesh quad_mesh;
};