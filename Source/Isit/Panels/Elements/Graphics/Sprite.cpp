#include "Sprite.hpp"

Sprite::Sprite() : 
    _scene(Service<Window>::get().scene()), 
    _canvas(CanvasEntity())
{
    // Setup
    _canvas.get()
        .ctx()
        .dimensions(CanvasContext::Dimensions::Absolute)
        .colors(CanvasContext::ColorFormat::Uint8);

    // Events
    _subscribe([&](const CommonEvents::MouseButton& btn) { 
        if (!__is_mouse_over)
            return;

        if (btn.button != MouseButton::Left) {
            return;
        }

        if (btn.action == InputAction::Pressed) {
            return onMousePressed();
        }

        if (btn.action == InputAction::Released) {
            return onMouseReleased();
        }
    });

    _subscribe([&](const CommonEvents::MouseMoved& mouse) { 
        // State changed
        if (__is_mouse_over != _hitArea(mouse.x, mouse.y)) {
            __is_mouse_over ^= true;
            __is_mouse_over ? onMouseOver() : onMouseOut();
        }

        if (__is_mouse_over) {
            return onMouseMoved(mouse.x, mouse.y);
        }
    });
}

Sprite::~Sprite() {
    clear();
    _children.clear();
    _unsubscribeAll();
}

void Sprite::draw() {
    clear();

    for (const auto& [name, sprite] : _children) {
        sprite->draw();
    }
}

void Sprite::clear() {
    for (const auto& [name, sprite] : _children) {
        sprite->clear();
    }
    _canvas.get().clear();
}

std::shared_ptr<Sprite> Sprite::get(const std::string& name) const {
    return _children.at(name);
}

void Sprite::add(const std::string& name, std::shared_ptr<Sprite> sprite) {
    _children.emplace(name, move(sprite));
}

float Sprite::width() const {
    return (float)_scene.width();
}
float Sprite::height() const {
    return (float)_scene.height();
}

bool Sprite::_hitArea(int /*x*/, int /*y*/) {
    return true;
}
