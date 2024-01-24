#include "Ui.hpp"

#include <glm/gtx/string_cast.hpp>


Ui::Ui(Scene& scene) : 
    m_scene(scene),
    m_state(Ui::State::Idle)
{
    // Create gui elements
    m_layout                 = std::make_shared<BaseLayout>(scene);
    m_widgets["StartButton"] = std::make_shared<ButtonWidget>("Start");

    // Define events
    _subscribe(m_layout.get(),                    &Ui::draw);
    _subscribe(m_widgets.at("StartButton").get(), &Ui::onClickStart);

    setState(Ui::State::Start);
}

void Ui::setState(Ui::State state) {
    if (m_state == state)
        return;

    m_state = state;

    // Change ui
    m_layout->clean();

    switch (m_state) {
    case State::Start:
        m_layout->opacity = 0.90f;
        m_layout->background_color = glm::vec4(0.1f, 0.1f, 0.15f, 1.0f);
        m_layout->add(m_widgets["StartButton"]);
        break;

    case State::InGame:
        m_layout->opacity = 1.0f;
        m_layout->background_color = BaseWidget::Transparent();
        break;
    }

    // Notify it changed
    Event::Emit(CommonEvents::StateUpdated(), this);
}

Ui::State Ui::state() const {
    return m_state;
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

void Ui::onClickStart(const BaseWidget* emitter, const CommonEvents::MouseButton& msg) 
{
    setState(State::InGame);
}
