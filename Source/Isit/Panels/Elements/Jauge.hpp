#pragma once

#include "Graphics/Sprite.hpp"
#include "Graphics/misc.hpp"

struct Jauge : public Sprite
{
    Jauge(const Rectangle& topLeft, const CanvasShape::Color& filled);
    virtual void draw() override;
    float& ratio();

protected:
    virtual bool _hitArea(int x, int y) override;

private:
    Rectangle _area;
    CanvasShape::Color _color;
    float _ratio;
};