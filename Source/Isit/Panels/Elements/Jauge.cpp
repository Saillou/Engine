#include "Jauge.hpp"

Jauge::Jauge(const Rectangle& rec, const CanvasShape::Color& filled) :
    _area(rec),
    _color(filled),
    _ratio(0.f)
{
}

void Jauge::draw() {
    clear();

    _canvas.get()
        .begin()
        .rect(_area.x, height() - _area.y - _area.h, _area.w * _ratio, _area.h)
        .fill(_color);

    _canvas.get()
        .begin()
        .rect(_area.x, height() - _area.y - _area.h, _area.w, _area.h)
        .stroke(glm::vec4(255, 255, 255, 255));
}

float& Jauge::ratio() {
    return _ratio;
}

bool Jauge::_hitArea(int x, int y) {
    return _area.is_in(x, y);
}