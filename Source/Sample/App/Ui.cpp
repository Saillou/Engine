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
    m_widget.opacity = 0.7f;
}

void Ui::draw() {
    m_scene.clear();
}

