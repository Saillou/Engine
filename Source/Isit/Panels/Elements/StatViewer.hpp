#pragma once

#include "Graphics/Sprite.hpp"
#include "Graphics/misc.hpp"

struct StatViewer : public Sprite
{
    StatViewer(const Point& topLeft, const std::string& text, const CanvasShape::Color& filled);

protected:
    virtual bool _hitArea(int x, int y) override;

private:
    void _state_updated(const CommonEvents::StateUpdated&);

    Point               _topLeft;
    CanvasShape::Color  _color;
};