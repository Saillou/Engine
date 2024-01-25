#include "ButtonWidget.hpp"

ButtonWidget::ButtonWidget(const std::string& text):
    _text(text),
    _backgroundColor(0.7f, 0.5f, 0.3f, 1.0f),
    _foregroundColor(1,1,1,1)
{
    glm::vec2 sizeMini = TextEngine::MeasureRel(_text, 0.5f);
    float padding = 0.05f;

    // Only 1 quad
    _surfaces.emplace_back(
        std::make_unique<Quad>(0.0f, 0.0f, sizeMini.x + padding, sizeMini.y + padding)
    );
}

void ButtonWidget::draw(Scene& scene) {
    auto& surface = _surfaces.front();

    // Update data
    surface->x() = x;
    surface->y() = y;
    surface->material.diffuse_color = isMouseOver() ? _foregroundColor : _backgroundColor;
    
    // Draw
    scene.renderer().quad(*surface);

    glm::vec2 size = TextEngine::Measure(_text, 0.5f);
    scene.renderer().text(_text,
        scene.width()  * (surface->x() + surface->w()/2.0f) - size.x/2.0f,
        scene.height() * (1.0f-surface->y() - surface->h()/2.0f) - size.y/2.0f,
        0.5f,
        isMouseOver() ? _backgroundColor : _foregroundColor
    );
}

std::string& ButtonWidget::text() {
    return _text;
}


float& ButtonWidget::w() {
    return _surfaces.front()->w();
}

float& ButtonWidget::h() {
    return _surfaces.front()->h();
}

