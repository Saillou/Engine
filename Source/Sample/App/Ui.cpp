#include "Ui.hpp"

#include <Engine/Graphic/Base/Widget/ButtonWidget.hpp>

Ui::Ui(Scene& scene) : 
    m_scene(scene),
    m_layout(scene)
{
    // Define events
    m_layout.on_draw() = [&]() {
        draw(); 
    };

    // Setup root widget
    m_layout.opacity = 1.0f;
    m_layout.add(std::make_shared<ButtonWidget>());
}

void Ui::draw() {
    if (!visible) {
        _cleanup();
        return;
    }

    m_scene.clear();

    // .. do something ..
}


void Ui::_cleanup() {
    if(m_layout.opacity > 0)
        m_layout.opacity -= 0.03f;
}