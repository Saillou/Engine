#include "Button.hpp"

ButtonWidget::ButtonWidget():
    _surface(0.45f, 0.45f, 0.1f, 0.1f),
    _text("Start"),
    _backgroundColor(0.7f, 0.5f, 0.3f, 1.0f),
    _foregroundColor(1,1,1,1)
{
}

void ButtonWidget::draw(Scene& scene) {
    _surface.draw(glm::vec4(0.7f, 0.4f, 0.2f, 1.0f));

    scene.renderer().text("Start", 
        scene.width()  * (_surface.x() + _surface.w()/3.0f),
        scene.height() * (_surface.y() + _surface.h()/2.5f)
    );
}
