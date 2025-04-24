#include "Button.hpp"

Button::Button(const Rectangle& r, const std::string& text, const CanvasShape::Color& primary) :
    _x(r.x), _y(r.y), _w(r.w), _h(r.h), 
    _text(text), 
    _primaryColor(primary.b, primary.g, primary.r, 32),
    _secondaryColor(primary.b, primary.g, primary.r, 255)
{
}

void Button::draw() {
    clear();

    _canvas.get()
        .begin()
        .rect(_x, _scene.height() - _y - _h, _w, _h)
        .fill(_primaryColor)
        .stroke(_is_over ? _secondaryColor : _primaryColor, 1.0f);

    _canvas.get()
        .begin()
        .text(_text, _x + 10.0f, _scene.height() - _y - _h + 50.0f, 0.5f)
        .fill(_is_over ? _secondaryColor : _primaryColor);
}

void Button::onMouseOver() {
    _is_over = true;
}
void Button::onMouseOut() {
    _is_over = false;
}
void Button::onMouseReleased() {
    Event::Emit(CommonEvents::MouseButton(MouseButton::Left, InputAction::Released), this);
}
void Button::onMousePressed() {
    Event::Emit(CommonEvents::MouseButton(MouseButton::Left, InputAction::Pressed), this);
}

bool Button::_hitArea(int mx, int my) {
    return (mx > _x && mx < _x + _w) && (my > _y && my < _y + _h);
}