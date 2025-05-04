#include "Button.hpp"

Button::Button(const Text& text):
    _rect(text.bounding_rect()),
    _text(text.text()),
    _primaryColor(text.color().b, text.color().g, text.color().r, 64),
    _secondaryColor(text.color().b, text.color().g, text.color().r, 255),
    _type(_Type::_rectangle)
{

}

Button::Button(const Rectangle& r, const std::string& text, const CanvasShape::Color& primary) :
    _rect(r),
    _text(text), 
    _primaryColor(primary.b, primary.g, primary.r, 64),
    _secondaryColor(primary.b, primary.g, primary.r, 255),
    _type(_Type::_rectangle)
{
}

Button::Button(const Circle& c, const std::string& text, const CanvasShape::Color& primary):
    _rect({ c.x, c.y, c.r, c.r }),
    _text(text),
    _primaryColor(primary.b, primary.g, primary.r, 64),
    _secondaryColor(primary.b, primary.g, primary.r, 255),
    _type(_Type::_circle)
{
}

void Button::draw() {
    clear();

    switch(_type) 
    {
    case _rectangle:
        _canvas.get()
            .begin()
            .rect(_rect.x, _scene.height() - _rect.y - _rect.h, _rect.w, _rect.h)
            .fill(_primaryColor)
            .stroke(_is_mouse_over() ? _secondaryColor : _primaryColor, 1.0f);
        break;

    case _circle:
        _canvas.get()
            .begin()
            .circle(_rect.x, _scene.height() - _rect.y - _rect.h, _rect.w/2.0f)
            .fill(_primaryColor)
            .stroke(_is_mouse_over() ? _secondaryColor : _primaryColor, 1.0f);
        break;
    }

    _canvas.get()
        .begin()
        .text(_text, _rect.x + 2.0f, _scene.height() - _rect.y - _rect.h + 17.0f, 0.5f)
        .fill(_is_mouse_over() ? _secondaryColor : _primaryColor);
}

void Button::onMouseReleased() {
    Event::Emit(CommonEvents::MouseButton(MouseButton::Left, InputAction::Released), this);
}
void Button::onMousePressed() {
    Event::Emit(CommonEvents::MouseButton(MouseButton::Left, InputAction::Pressed), this);
}

bool Button::_hitArea(int mx, int my) {
    return _rect.is_in(mx, my);
}