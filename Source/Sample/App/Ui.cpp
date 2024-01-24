#include "Ui.hpp"

#include <glm/gtx/string_cast.hpp>

#include <Engine/Graphic/Base/Widget/ButtonWidget.hpp>


Ui::Ui(Scene& scene) : 
    m_scene(scene),
    m_layout(scene),
    m_state(Ui::State::Start)
{
    // Define events
    _subscribe(&m_layout, &Ui::draw);

    setState(m_state);
}

void Ui::setState(Ui::State state) {
    m_state = state;

    switch (m_state) {
    case State::Start:
        m_layout.opacity = 0.95f;
        m_layout.background_color = glm::vec4(0.1f, 0.1f, 0.15f, 1.0f);
        m_layout.add(ButtonWidget::Create());
        break;

    case State::InGame:
        m_layout.opacity = 1.0f;
        m_layout.background_color = BaseWidget::Transparent();
        m_layout.clean();
        break;
    }
}

void Ui::draw(const BaseLayout* emitter, const LayoutEvents::Draw& msg) {
    switch (m_state) {
    case State::Start:
        break;

    case State::InGame:
        auto& renderer = m_scene.renderer();
        const float w = (float)m_scene.width();
        const float h = (float)m_scene.height();

        renderer.text("Cam pos: " + glm::to_string(m_scene.camera().position), 15.0f, h - 20.0f, 0.4f);
        renderer.text("Cam dir: " + glm::to_string(m_scene.camera().direction), 15.0f, h - 40.0f, 0.4f);
        break;
    }
}
