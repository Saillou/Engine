#include "Ui.hpp"

#include <Engine/Graphic/Base/Widget/ButtonWidget.hpp>

Ui::Ui(Scene& scene) : 
    m_scene(scene),
    m_layout(scene)
{
    // Define events
    _subscribe(&m_layout, &Ui::draw);

    // Setup root widget
    m_layout.opacity = 1.0f;
    m_layout.add(ButtonWidget::Create());
}

void Ui::draw(const BaseLayout* emitter, const LayoutEvents::Draw& msg) {
    if (!visible) {
        _cleanup();
        return;
    }

    // .. do something ..
}


void Ui::_cleanup() {
    if(m_layout.opacity > 0)
        m_layout.opacity -= 0.03f;
}