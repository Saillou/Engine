#pragma once

#include "BaseShape.hpp"
#include "../../Cookable.hpp"

struct Quad :
    public BaseShape,
    public Cookable
{
    Quad(float x0 = .0f, float y0 = .0f, float width = 1.f, float height = 1.f);
    ~Quad() = default;

    void draw();
    void drawElements();
};