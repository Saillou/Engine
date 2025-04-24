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
        if (__is_mouse_over == _hitArea(mouse.x, mouse.y))
            return; // nothing's changed

        __is_mouse_over ^= true;

        return __is_mouse_over ? onMouseOver() : onMouseOut();
    });
}

void Sprite::clear() {
    _canvas.get()
        .clear();
}