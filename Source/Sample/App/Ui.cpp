#include "Ui.hpp"

Ui::Ui(Scene& scene) : 
    m_scene(scene),
    m_widget(scene)
{
    // Define events
    m_widget.on_draw() = [&]() { 
        draw(); 
    };

    // Setup root widget
    m_widget.opacity = 1.0f;
}

void Ui::draw() {
    if (!visible) {
        _cleanup();
        return;
    }

    m_scene.clear();
    m_button_start.draw(m_scene);
}


void Ui::_cleanup() {
    if(m_widget.opacity > 0)
        m_widget.opacity -= 0.03f;
}