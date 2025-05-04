#pragma once

#include "Graphics/Sprite.hpp"
#include "Graphics/misc.hpp"

struct Text : public Sprite
{
    Text(const Point& topLeft, float _size, const std::string& text, const CanvasShape::Color& primary);
    virtual void draw() override;
    std::string& text();
    const std::string& text() const;
    Rectangle bounding_rect(float margin = 5.0f) const;
    const CanvasShape::Color& color() const;

protected:
    virtual bool _hitArea(int x, int y) override;

private:
    CanvasShape::Color _color;
    Point _topLeft;
    std::string _text;
    float _size;
};