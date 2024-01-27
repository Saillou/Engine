#include "Ui.hpp"

#include <glm/gtx/string_cast.hpp>

// -- Ui --
Ui::Ui(Scene& scene) : 
    m_scene(scene),
    m_prev_state(Ui::State::Idle),
    m_state(Ui::State::Idle)
{
    m_layout = std::make_shared<Layout>(scene);

    // Classnames
    static constexpr char DefaultButton[] = ".DefaultButton";
    static constexpr char SmallButton[]   = ".SmallButton";
    static constexpr char Title[]         = ".Title";

    // Setup initial style
    m_layout->styleSheet() = StyleSheet::CreateDefault()
        .addRule(Style::Tag::Layout, Style{ }
            .set_opacity(0.90f)
            .set_background(glm::vec4(0.1f, 0.1f, 0.15f, 1.0f))
        )
        .addRule(Style::Tag::Button, Style{ }
            .set_background(glm::vec4(0.15f, 0.15f, 0.17f, 1.0f))
            .set_foreground(glm::vec4(0.66f, 0.66f, 0.70f, 1.0f))
        )
        .addRule(Style::Tag::Text, Style{ }
            .set_foreground(glm::vec4(0.90f, 0.90f, 0.93f, 1.0f))
        )
        .addRule(DefaultButton, Style{ }
            .set_hint_w(0.1f)
        )
        .addRule(SmallButton, Style{ }
            .set_hint_w(0.03f)
            .set_hint_h(0.04f)
        )
        .addRule(Title, Style{ }
            .set_textSize(0.8f)
        )
    ;

    // - Create gui elements
    // Buttons
    m_buttons["Start"]  = Widget::WithClass<DefaultButton>::Create<Button>("Start");
    m_buttons["Option"] = Widget::WithClass<DefaultButton>::Create<Button>("Option");
    m_buttons["Resume"] = Widget::WithClass<DefaultButton>::Create<Button>("Resume");
    m_buttons["Close"]  = Widget::WithClass<DefaultButton>::Create<Button>("Quit");

    m_buttons["Moins"]  = Widget::WithClass<SmallButton>::Create<Button>("-");
    m_buttons["Plus"]   = Widget::WithClass<SmallButton>::Create<Button>("+");

    m_buttons["Apply"]  = Widget::Create<Button>("Apply");

    // Texts
    m_texts["Title"]    = Widget::WithClass<Title>::Create<Text>("The Game");
    m_texts["Pause"]    = Widget::WithClass<Title>::Create<Text>("Game paused");
    m_texts["Option"]   = Widget::WithClass<Title>::Create<Text>("Options");
    m_texts["Lights"]   = Widget::WithClass<Title>::Create<Text>("Lights");
    m_texts["Count"]    = Widget::WithClass<Title>::Create<Text>(std::to_string(m_lights_count));

    m_texts["Ig1"]      = Widget::Create<Text>("");
    m_texts["Ig2"]      = Widget::Create<Text>(Text::Block{
        "Press [space] to pause",
        "Press [R] to dis/enable filters",
        "Press [T] to dis/enable casters"
    });

    // Define events
    _subscribe(m_layout.get(), &Ui::draw);

    _subscribe(m_buttons.at("Option"), [=](const CommonEvents::MouseButton&) { setState(State::Option); });
    _subscribe(m_buttons.at("Start"),  [=](const CommonEvents::MouseButton&) { setState(State::InGame); });
    _subscribe(m_buttons.at("Resume"), [=](const CommonEvents::MouseButton&) { setState(State::InGame); });
    _subscribe(m_buttons.at("Apply"),  [=](const CommonEvents::MouseButton&) { setState(m_prev_state);  });
    _subscribe(m_buttons.at("Moins"),  [=](const CommonEvents::MouseButton&) { _updateCount(-1);        });
    _subscribe(m_buttons.at("Plus"),   [=](const CommonEvents::MouseButton&) { _updateCount(+1);        });
    _subscribe(m_buttons.at("Close"),  [=](const CommonEvents::MouseButton&) { wantQuit = true;         });

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
        m_layout->add(m_texts["Title"],    .45f, .31f);

        m_layout->add(m_buttons["Start"],  .45f, .38f);
        m_layout->add(m_buttons["Option"], .45f, .45f);
        m_layout->add(m_buttons["Close"],  .45f, .70f);
        break;

    case State::Option:
        m_layout->add(m_texts["Option"],  .46f, .32f);

        m_layout->add(m_texts["Lights"],  .40f,  .51f);
        m_layout->add(m_buttons["Moins"], .47f,  .465f);
        m_layout->add(m_texts["Count"],   .515f, .51f);
        m_layout->add(m_buttons["Plus"],  .54f,  .465f);

        m_layout->add(m_buttons["Apply"], .45f, .59f);
        break;

    case State::InGame:
        // Note: it will override previous style
        m_layout->style().set_opacity(1.0f);
        m_layout->style().set_background(Style::Transparent());

        m_layout->add(m_texts["Ig1"], .01f, .03f);
        m_layout->add(m_texts["Ig2"], .01f, .10f);
        break;

    case State::Pause:
        m_layout->add(m_buttons["Resume"], .45f, .38f);
        m_layout->add(m_buttons["Option"], .45f, .45f);
        m_layout->add(m_texts["Pause"],    .43f, .30f);

        m_layout->add(m_buttons["Close"],  .45f, .70f);
        break;
    }

    // Notify it changed
    Event::Emit(CommonEvents::StateUpdated(), this);
}

Ui::State Ui::state() const {
    return m_state;
}

int Ui::getLightsCount() const {
    return m_lights_count;
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
    if (m_lights_count + delta < 0 || m_lights_count + delta > 5)
        return;

    m_lights_count += delta;

    m_texts["Count"]->setText(std::to_string(m_lights_count));
}
