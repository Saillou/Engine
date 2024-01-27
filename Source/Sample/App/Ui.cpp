#include "Ui.hpp"

#include <Engine/Graphic/Base/Widget/Button.hpp>
#include <Engine/Graphic/Base/Widget/Text.hpp>
#include <Engine/Graphic/Base/Widget/Layout/VerticalLayout.hpp>
#include <Engine/Graphic/Base/Widget/Layout/HorizontalLayout.hpp>

#include <glm/gtx/string_cast.hpp>

// -- Ui --
Ui::Ui(Scene& scene) : 
    m_scene(scene),
    m_prev_state(Ui::State::Idle),
    m_state(Ui::State::Idle)
{
    // Classnames
    static constexpr char Title[]         = ".Title";
    static constexpr char SmallButton[]   = ".SmallButton";
    static constexpr char DefaultButton[] = ".DefaultButton";

    // - Create gui elements
    std::unordered_map<std::string, std::shared_ptr<Button>> buttons;
    std::unordered_map<std::string, std::shared_ptr<Text>> texts;

    // Buttons
    buttons["Start"]  = Widget::WithClass<DefaultButton>::Create<Button>("Start");
    buttons["Option"] = Widget::WithClass<DefaultButton>::Create<Button>("Option");
    buttons["Resume"] = Widget::WithClass<DefaultButton>::Create<Button>("Resume");
    buttons["Close"]  = Widget::WithClass<DefaultButton>::Create<Button>("Quit");

    buttons["Moins"]  = Widget::WithClass<SmallButton>::Create<Button>("-");
    buttons["Plus"]   = Widget::WithClass<SmallButton>::Create<Button>("+");

    buttons["Apply"]  = Widget::Create<Button>("Apply");

    // Texts
    texts["Title"]    = Widget::WithClass<Title>::Create<Text>("The Game");
    texts["Pause"]    = Widget::WithClass<Title>::Create<Text>("Game paused");
    texts["Option"]   = Widget::WithClass<Title>::Create<Text>("Options");

    texts["Lights"]   = Widget::Create<Text>("Lights");
    texts["Count"]    = Widget::Create<Text>(std::to_string(m_lights_count));
    texts["Ig1"]      = Widget::Create<Text>("");
    texts["Ig2"]      = Widget::Create<Text>(Text::Block{
        "Press [space] to pause",
        "Press [R] to dis/enable filters",
        "Press [T] to dis/enable casters"
    });

    // - Create layouts
    m_layouts["Main"] = Widget::Create<MainLayout>(scene);
    m_layouts["Main"]->styleSheet() = StyleSheet::CreateDefault();

    // Start
    m_layouts["Start"] = Widget::Create<VerticalLayout>(scene);
    {
        m_layouts["Start"]->add(texts["Title"], .45f, .31f, "Title");
        m_layouts["Start"]->add(buttons["Start"], .45f, .38f, "Start");
        m_layouts["Start"]->add(buttons["Option"], .45f, .45f, "Option");
        m_layouts["Start"]->add(buttons["Close"], .45f, .70f, "Close");
    }

    // Options
    m_layouts["Option"] = Widget::Create<VerticalLayout>(scene);
    {
        m_layouts["LLight"] = Widget::Create<HorizontalLayout>(scene);
        {
            m_layouts["LLight"]->add(texts["Lights"], .45f, .50f, "Lights");
            m_layouts["LLight"]->add(buttons["Moins"], .49f, .47f, "Moins");
            m_layouts["LLight"]->add(texts["Count"], .515f, .50f, "Count");
            m_layouts["LLight"]->add(buttons["Plus"], .53f, .47f, "Plus");
        }

        m_layouts["Option"]->add(texts["Option"], .46f, .32f, "Option");
        m_layouts["Option"]->add(m_layouts["LLight"], .45f, .59f, "LLight");
        m_layouts["Option"]->add(buttons["Apply"], .45f, .59f, "Apply");
    }

    // In game
    m_layouts["InGame"] = Widget::Create<Layout>(scene);
    {
        m_layouts["InGame"]->add(texts["Ig1"], .01f, .03f, "Ig1");
        m_layouts["InGame"]->add(texts["Ig2"], .01f, .10f, "Ig2");
    }

    // Pause
    m_layouts["Pause"] = Widget::Create<VerticalLayout>(scene);
    {
        m_layouts["Pause"]->add(buttons["Resume"], .45f, .38f, "Resume");
        m_layouts["Pause"]->add(buttons["Option"], .45f, .45f, "Option");
        m_layouts["Pause"]->add(texts["Pause"], .43f, .30f, "Pause");
        m_layouts["Pause"]->add(buttons["Close"], .45f, .70f, "Close");
    }

    // Define events
    _subscribe(m_layouts.at("Main"), &Ui::draw);

    _subscribe(buttons.at("Option"), [=](const CommonEvents::MouseButton&) { setState(State::Option); });
    _subscribe(buttons.at("Start"),  [=](const CommonEvents::MouseButton&) { setState(State::InGame); });
    _subscribe(buttons.at("Resume"), [=](const CommonEvents::MouseButton&) { setState(State::InGame); });
    _subscribe(buttons.at("Apply"),  [=](const CommonEvents::MouseButton&) { setState(m_prev_state);  });
    _subscribe(buttons.at("Moins"),  [=](const CommonEvents::MouseButton&) { _updateCount(-1);        });
    _subscribe(buttons.at("Plus"),   [=](const CommonEvents::MouseButton&) { _updateCount(+1);        });
    _subscribe(buttons.at("Close"),  [=](const CommonEvents::MouseButton&) { wantQuit = true;         });

    // Create a custom style
    m_layouts["Main"]->styleSheet()
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
            .set_hint_w(0.020f)
            .set_hint_h(0.035f)
        )
        .addRule(Title, Style{ }
            .set_textSize(0.8f)
        )
    ;

    // Let's start
    setState(Ui::State::Start);
}

void Ui::setState(Ui::State state) {
    if (m_state == state)
        return;

    m_prev_state = m_state;
    m_state = state;

    // Change ui
    m_layouts["Main"]->clean();

    switch (m_state) {
    case State::Start:
        m_layouts["Main"]->add(m_layouts["Start"], "Start");
        break;

    case State::Option:
        m_layouts["Main"]->add(m_layouts["Option"], "Option");
        break;

    case State::InGame:
        // Note: it will override previous style
        m_layouts["Main"]->style().set_opacity(1.0f);
        m_layouts["Main"]->style().set_background(Style::Transparent());

        m_layouts["Main"]->add(m_layouts["InGame"], "InGame");
        break;

    case State::Pause:
        m_layouts["Main"]->add(m_layouts["Pause"], "Pause");
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
        m_layouts["InGame"]->find<Text>("Ig1")->at(0) = "Cam pos: " + glm::to_string(m_scene.camera().position);
        m_layouts["InGame"]->find<Text>("Ig1")->at(1) = "Cam dir: " + glm::to_string(m_scene.camera().direction);
        break;
    }
}

void Ui::_updateCount(int delta) {
    if (m_lights_count + delta < 0 || m_lights_count + delta > 5)
        return;

    m_lights_count += delta;
    m_layouts["LLight"]->find<Text>("Count")->setText(std::to_string(m_lights_count));
}
