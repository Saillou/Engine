#include "Ui.hpp"

#include <glm/gtx/string_cast.hpp>


Ui::Ui(Scene& scene) : 
    m_scene(scene),
    m_state(Ui::State::Idle)
{
    m_layout = std::make_shared<BaseLayout>(scene);

    // Create gui elements
    m_widgets["StartButton"] = std::make_shared<ButtonWidget>("Start");
    m_widgets["StartButton"]->x = 0.45f;
    m_widgets["StartButton"]->y = 0.38f;

    m_widgets["OptionButton"] = std::make_shared<ButtonWidget>("Option");
    m_widgets["OptionButton"]->x = 0.45f;
    m_widgets["OptionButton"]->y = 0.45f;

    m_widgets["ResumeButton"] = std::make_shared<ButtonWidget>("Resume");
    m_widgets["ResumeButton"]->x = 0.45f;
    m_widgets["ResumeButton"]->y = 0.45f;

    m_widgets["MoinsButton"] = std::make_shared<ButtonWidget>("o");
    m_widgets["MoinsButton"]->x = 0.41f;
    m_widgets["MoinsButton"]->y = 0.47f;

    m_widgets["PlusButton"] = std::make_shared<ButtonWidget>("o");
    m_widgets["PlusButton"]->x = 0.52f;
    m_widgets["PlusButton"]->y = 0.47f;

    // Some overrides
    std::dynamic_pointer_cast<ButtonWidget>(m_widgets["StartButton"])->w()    = std::dynamic_pointer_cast<ButtonWidget>(m_widgets["OptionButton"])->w();
    std::dynamic_pointer_cast<ButtonWidget>(m_widgets["MoinsButton"])->text() = "-";
    std::dynamic_pointer_cast<ButtonWidget>(m_widgets["PlusButton"])->text()  = "+";

    // Define events
    _subscribe(m_layout.get(), &Ui::draw);

    _subscribe(m_widgets.at("OptionButton").get(), &Ui::onClickOption);
    _subscribe(m_widgets.at("StartButton").get(),  &Ui::onClickStart);
    _subscribe(m_widgets.at("ResumeButton").get(), &Ui::onClickStart);

    // Let's start
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
        m_layout->add(m_widgets["OptionButton"]);
        break;

    case State::Option:
        m_layout->opacity = 0.90f;
        m_layout->background_color = glm::vec4(0.1f, 0.1f, 0.15f, 1.0f);
        m_layout->add(m_widgets["MoinsButton"]);
        m_layout->add(m_widgets["PlusButton"]);
        break;

    case State::InGame:
        m_layout->opacity = 1.0f;
        m_layout->background_color = BaseWidget::Transparent();
        break;

    case State::Pause:
        m_layout->opacity = 0.90f;
        m_layout->background_color = glm::vec4(0.1f, 0.1f, 0.15f, 1.0f);
        m_layout->add(m_widgets["ResumeButton"]);
        break;
    }

    // Notify it changed
    Event::Emit(CommonEvents::StateUpdated(), this);
}

Ui::State Ui::state() const {
    return m_state;
}

void Ui::draw(const LayoutEvents::Draw& msg) {
    auto& renderer = m_scene.renderer();
    const float w = (float)m_scene.width();
    const float h = (float)m_scene.height();

    glm::vec2 textSize;

    switch (m_state) {
    case State::Start:
        textSize = TextEngine::Measure("The Game", 0.7f);
        renderer.text("The Game", w / 2.0f - textSize.x / 2.0f, h / 2.0f + 150.0f, 0.7f);
        break;

    case State::Option:
        textSize = TextEngine::Measure("Options", 0.7f);
        renderer.text("Options", w / 2.0f - textSize.x / 2.0f, h / 2.0f + 100.0f, 0.7f);
        renderer.text(std::to_string(m_count), w/2.0f - 20.f, h/2.0f -10.f, 0.7f);
        break;

    case State::InGame:
        renderer.text("Cam pos: " + glm::to_string(m_scene.camera().position), 15.0f, h - 20.0f, 0.4f);
        renderer.text("Cam dir: " + glm::to_string(m_scene.camera().direction), 15.0f, h - 40.0f, 0.4f);

        renderer.text("Press [space] to pause", 15.0f, h - 160.0f, 0.4f);
        renderer.text("Press [R] to dis/enable filters", 15.0f, h - 180.0f, 0.4f);
        renderer.text("Press [T] to dis/enable casters", 15.0f, h - 200.0f, 0.4f);
        renderer.text("Press [Numpad+(0-5)] to change lights", 15.0f, h - 220.0f, 0.4f);
        break;

    case State::Pause:
        textSize = TextEngine::Measure("Game paused", 0.7f);
        renderer.text("Game paused", w/2.0f - textSize.x/2.0f, h/2.0f + 100.0f, 0.7f);
        break;
    }
}

void Ui::onClickStart(const CommonEvents::MouseButton& msg) {
    setState(State::InGame);
}

void Ui::onClickOption(const CommonEvents::MouseButton& msg) {
    setState(State::Option);
}

