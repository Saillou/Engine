#include "Ui.hpp"

#include <glm/gtx/string_cast.hpp>

Ui::Ui(Scene& scene) : 
    m_scene(scene),
    m_prev_state(Ui::State::Idle),
    m_state(Ui::State::Idle)
{
    m_layout = std::make_shared<BaseLayout>(scene);

    // - Create gui elements -
    m_buttons["Start"]  = std::make_shared<ButtonWidget>("Start");
    m_buttons["Option"] = std::make_shared<ButtonWidget>("Option");
    m_buttons["Resume"] = std::make_shared<ButtonWidget>("Resume");
    m_buttons["Quit"]   = std::make_shared<ButtonWidget>("Quit");
    m_buttons["Apply"]  = std::make_shared<ButtonWidget>("Apply");
    m_buttons["Moins"]  = std::make_shared<ButtonWidget>("-");
    m_buttons["Plus"]   = std::make_shared<ButtonWidget>("+");

    m_texts["Title"]  = std::make_shared<TextWidget>("The Game",              0.7f);
    m_texts["Pause"]  = std::make_shared<TextWidget>("Game paused",           0.7f);
    m_texts["Option"] = std::make_shared<TextWidget>("Options",               0.7f);
    m_texts["Count"]  = std::make_shared<TextWidget>(std::to_string(m_count), 0.7f);
    m_texts["Ig1"]    = std::make_shared<TextWidget>(std::vector<std::string>{
        "Cam pos: ",
        "Cam dir: " 
    }, 0.4f);
    m_texts["Ig2"]    = std::make_shared<TextWidget>(std::vector<std::string>{
        "Press [space] to pause",
        "Press [R] to dis/enable filters",
        "Press [T] to dis/enable casters",
        "Press [Numpad+(0-5)] to change lights"
    }, 0.4f);

    // Some overrides
    const float default_width = m_buttons["Option"]->w();
    m_buttons["Start"]->w()   = default_width;
    m_buttons["Quit"]->w()    = default_width;
    m_buttons["Resume"]->w()  = default_width;

    m_buttons["Moins"]->w() = m_buttons["Plus"]->w();
    m_buttons["Moins"]->h() = m_buttons["Plus"]->h();

    // Define events
    _subscribe(m_layout.get(), &Ui::draw);

    _subscribe(m_buttons.at("Option").get(), [=](const CommonEvents::MouseButton&) {
        setState(State::Option);
    });
    _subscribe(m_buttons.at("Apply").get(), [=](const CommonEvents::MouseButton&) {
        setState(m_prev_state);
    });
    _subscribe(m_buttons.at("Start").get(), [=](const CommonEvents::MouseButton&) {
        setState(State::InGame);
    });
    _subscribe(m_buttons.at("Resume").get(), [=](const CommonEvents::MouseButton&) {
        setState(State::InGame);
    });
    _subscribe(m_buttons.at("Quit").get(), [=](const CommonEvents::MouseButton&) {
        wantQuit = true;
    });
    _subscribe(m_buttons.at("Moins").get(), [=](const CommonEvents::MouseButton&) {
        _updateCount(-1);
    });
    _subscribe(m_buttons.at("Plus").get(), [=](const CommonEvents::MouseButton&) {
        _updateCount(+1);
    });

    // Let's start
    setState(Ui::State::Start);
}

void Ui::setState(Ui::State state) {
    if (m_state == state)
        return;

    m_prev_state = m_state;
    m_state = state;

    // Change ui
    m_layout->clean();

    switch (m_state) {
    case State::Start:
        m_layout->opacity = 0.90f;
        m_layout->background_color = glm::vec4(0.1f, 0.1f, 0.15f, 1.0f);
        m_layout->add(m_buttons["Start"],  .45f,  .38f);
        m_layout->add(m_buttons["Option"], .45f,  .45f);
        m_layout->add(m_buttons["Quit"],   .45f,  .55f);
        m_layout->add(m_texts["Title"],    .455f, .31f);
        break;

    case State::Option:
        m_layout->opacity = 0.90f;
        m_layout->background_color = glm::vec4(0.1f, 0.1f, 0.15f, 1.0f);
        m_layout->add(m_buttons["Apply"], .45f, .55f);
        m_layout->add(m_buttons["Moins"], .41f, .47f);
        m_layout->add(m_buttons["Plus"],  .52f, .47f);
        m_layout->add(m_texts["Option"],  .46f, .32f);
        m_layout->add(m_texts["Count"],   .49f, .51f);
        break;

    case State::InGame:
        m_layout->opacity = 1.0f;
        m_layout->background_color = BaseWidget::Transparent();
        m_layout->add(m_texts["Ig1"], .01f, .03f);
        m_layout->add(m_texts["Ig2"], .01f, .10f);
        break;

    case State::Pause:
        m_layout->opacity = 0.90f;
        m_layout->background_color = glm::vec4(0.1f, 0.1f, 0.15f, 1.0f);
        m_layout->add(m_buttons["Resume"], .45f, .38f);
        m_layout->add(m_buttons["Option"], .45f, .45f);
        m_layout->add(m_buttons["Quit"],   .45f, .70f);
        m_layout->add(m_texts["Pause"],    .44f, .30f);
        break;
    }

    // Notify it changed
    Event::Emit(CommonEvents::StateUpdated(), this);
}

Ui::State Ui::state() const {
    return m_state;
}

void Ui::draw(const LayoutEvents::Draw& msg) {
    switch (m_state) {
    case State::InGame:
        m_texts["Ig1"]->at(0) = "Cam pos: " + glm::to_string(m_scene.camera().position);
        m_texts["Ig1"]->at(1) = "Cam dir: " + glm::to_string(m_scene.camera().direction);
        break;
    }
}

void Ui::_updateCount(int delta) {
    if (m_count + delta < 0 || delta + delta > 5)
        return;

    m_count += delta;

    const auto color_normal = glm::vec4(0.7f, 0.5f, 0.3f, 1.0f);
    const auto color_danger = glm::vec4(1.0f, 0.3f, 0.3f, 1.0f);
    const auto color_white  = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    const auto color_gray   = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

    m_buttons["Plus"]->backgroundColor  = (m_count >= 5) ? color_danger : color_normal;
    m_buttons["Plus"]->foregroundColor  = (m_count >= 5) ? color_gray   : color_white;
    m_buttons["Moins"]->backgroundColor = (m_count <= 0) ? color_danger : color_normal;
    m_buttons["Moins"]->foregroundColor = (m_count <= 0) ? color_gray   : color_white;

    m_texts["Count"]->setText(std::to_string(m_count));
}
