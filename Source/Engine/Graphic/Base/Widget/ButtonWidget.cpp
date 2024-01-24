#include "ButtonWidget.hpp"

ButtonWidget::ButtonWidget(const std::string& text):
    _surface(0.45f, 0.45f, 0.1f, 0.1f),
    _text(text),
    _backgroundColor(0.7f, 0.5f, 0.3f, 1.0f),
    _foregroundColor(1,1,1,1)
{
}

void ButtonWidget::draw(Scene& scene) {
    _surface.draw(_backgroundColor);

    scene.renderer().text(_text,
        scene.width()  * (_surface.x() + _surface.w()/3.0f),
        scene.height() * (_surface.y() + _surface.h()/2.5f),
        0.5f,
        _foregroundColor
    );
}
