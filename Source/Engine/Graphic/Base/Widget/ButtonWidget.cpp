#include "ButtonWidget.hpp"

ButtonWidget::ButtonWidget(const std::string& text):
    _text(text),
    _backgroundColor(0.7f, 0.5f, 0.3f, 1.0f),
    _foregroundColor(1,1,1,1)
{
    // Only 1 quad
    _surfaces.emplace_back(
        std::make_unique<Quad>(0.45f, 0.45f, 0.1f, 0.1f)
    );
}

void ButtonWidget::draw(Scene& scene) {
    if (_surfaces.empty())
        return;

    auto& surface = _surfaces.front();

    surface->draw(_backgroundColor);

    scene.renderer().text(_text,
        scene.width()  * (surface->x() + surface->w()/3.0f),
        scene.height() * (surface->y() + surface->h()/2.5f),
        0.5f,
        _foregroundColor
    );
}
