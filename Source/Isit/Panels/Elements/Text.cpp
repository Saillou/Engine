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

bool Text::_hitArea(int x, int y) {
    return false;
}
