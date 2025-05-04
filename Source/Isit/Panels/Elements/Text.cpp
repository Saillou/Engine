#include "Text.hpp"

Text::Text(const Point& pt, float size, const std::string& text, const CanvasShape::Color& primary) :
    _topLeft(pt),
    _size(size),
    _text(text), 
    _color(primary)
{
}

void Text::draw() {
    clear();

    _canvas.get()
        .begin()
        .text(_text, _topLeft.x, height() - _topLeft.y, _size)
        .fill(_color);
}

std::string& Text::text() {
    return _text;
}

const std::string& Text::text() const {
    return _text;
}

Rectangle Text::bounding_rect(float margin) const
{
    auto text_measure = TextEngine::Measure(_text, _size);
    float x = _topLeft.x - margin;
    float y = _topLeft.y - text_measure.y + margin;
    float w = text_measure.x + 2 * margin;
    float h = text_measure.y + 2 * margin;

    return Rectangle{ x, y, w, h };
}

const CanvasShape::Color& Text::color() const {
    return _color;
}

bool Text::_hitArea(int xMouse, int yMouse) {
    return bounding_rect().is_in(xMouse, yMouse);
}
