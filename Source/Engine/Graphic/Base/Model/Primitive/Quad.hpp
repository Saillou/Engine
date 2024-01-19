#pragma once

#include "PrimitiveHelper.hpp"
#include "../Mesh.hpp"
#include "../../../Utils/Shader.hpp"

struct Quad :
    private PrimitiveHelper
{
    Quad(float x0 = .0f, float y0 = .0f, float width = 1.f, float height = 1.f);
    ~Quad() = default;

    void draw();
    void drawElements();

private:
    Mesh quad_mesh;
    Shader shader_quad;
};