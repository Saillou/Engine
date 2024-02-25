#include "UiCube.hpp"
#include <glm/gtx/string_cast.hpp>
#include <Engine/Framework/Service.hpp>
#include <Engine/Graphic/Window.hpp>

// Classnames
static constexpr char Title[]         = ".Title";
static constexpr char Info[]          = ".Information";
static constexpr char SmallButton[]   = ".SmallButton";
static constexpr char DefaultButton[] = ".DefaultButton";

// -- Ui --
UiCube::UiCube():
    m_prev_state(UiCube::State::Idle),
    m_state(UiCube::State::Idle),
    m_main_frame(Service<Window>::get().scene())
{
    // Temporary container for gui elements
    std::unordered_map<std::string, std::shared_ptr<Button>> btns;
    std::unordered_map<std::string, std::shared_ptr<Text>>   txts;

    // Create layouts
    m_main_frame.layout().styleSheet() = _create_style();
    _create_widgets(btns, txts);
    _create_layouts(btns, txts);
    
    // Define events
    _subscribe(&m_main_frame, &UiCube::draw);

    _subscribe(btns.at("Start"),  [=](const WidgetEvents::MouseClick&) { setState(State::InGame); });
    _subscribe(btns.at("Resume"), [=](const WidgetEvents::MouseClick&) { setState(State::InGame); });
    _subscribe(btns.at("Option"), [=](const WidgetEvents::MouseClick&) { setState(State::Option); });
    _subscribe(btns.at("Apply"),  [=](const WidgetEvents::MouseClick&) { setState(m_prev_state);  });
    _subscribe(btns.at("Moins"),  [=](const WidgetEvents::MouseClick&) { _updateCount(-1);        });
    _subscribe(btns.at("Plus"),   [=](const WidgetEvents::MouseClick&) { _updateCount(+1);        });
    _subscribe(btns.at("Close"),  [=](const WidgetEvents::MouseClick&) { m_wantQuit = true;       });

    // Let's start
    setState(UiCube::State::Start);
}

void UiCube::setState(UiCube::State state) {
    if (m_state == state)
        return;

    m_prev_state = m_state;
    m_state = state;

    // Change ui
    m_main_frame.layout().clean();

    switch (m_state) {
    case State::Start:
        m_main_frame.layout().add(m_layouts["Start"]);
        break;

    case State::Option:
        m_main_frame.layout().add(m_layouts["Option"]);
        break;

    case State::InGame:
        m_main_frame.layout().style().background.setValue(Style::Transparent());
        m_main_frame.layout().add(m_layouts["InGame"]);
        break;

    case State::Pause:
        m_main_frame.layout().style().background.setValue(glm::vec4(0.1f, 0.1f, 0.15f, 0.5f));
        m_main_frame.layout().add(m_layouts["Pause"]);
        break;
    }

    // Notify it changed
    Event::Emit(CommonEvents::StateUpdated(), this);
}

UiCube::State UiCube::state() const {
    return m_state;
}

int UiCube::lightsCount() const {
    return m_lights_count;
}

bool UiCube::wantQuit() const {
    return m_wantQuit;
}

void UiCube::draw(const WidgetEvents::Draw& msg) {
    Scene& scene = Service<Window>::get().scene();

    switch (m_state) {
    case State::InGame:
        m_layouts["InGame"]->find<Text>("#Ig1")->at(0) = "Cam pos: " + glm::to_string(scene.camera.position);
        m_layouts["InGame"]->find<Text>("#Ig1")->at(1) = "Cam dir: " + glm::to_string(scene.camera.direction);
        break;
    }
}

void UiCube::_updateCount(int delta) {
    if (m_lights_count + delta < 0 || m_lights_count + delta > 5)
        return;

    m_lights_count += delta;
    m_layouts["LLight"]->find<Text>("#Count")->setText(std::to_string(m_lights_count));
}

// ---------- Helpers ----------
StyleSheet UiCube::_create_style() const
{
    const glm::vec4 primary_color    = glm::vec4(0.1f, 0.1f, 0.15f, 0.7f);
    const glm::vec4 secondary_color  = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    const glm::vec4 main_text_color  = glm::vec4(0.90f, 0.90f, 0.93f, 1.0f);
    const glm::vec4 foreground_color = glm::vec4(0.66f, 0.66f, 0.70f, 1.0f);

    return StyleSheet::CreateDefault()
        .addRule(Style::Tag::Layout, Style()
            .background.setValue(primary_color)
        )
        .addRule(Style::Tag::Button, Style()
            .background.setValue(secondary_color)
            .foreground.setValue(foreground_color)
            .contentAlign.setValue({
                Style::Align::Centered, 
                Style::Align::Centered 
            })
        )
        .addRule(Style::Tag::Text, Style()
            .foreground.setValue(main_text_color)
        )
        .addRule(DefaultButton, Style()
            .hint_w.setValue(0.1f)
        )
        .addRule(SmallButton, Style()
            .hint_w.setValue(0.020f)
            .hint_h.setValue(0.035f)
        )
        .addRule(Title, Style()
            .textSize.setValue(1.0f)
        )
        .addRule(Info, Style()
            .textSize.setValue(0.35f)
        )
        .addRule("#Count", Style()
            .foreground.setValue(foreground_color)
        )
    ;
}

void UiCube::_create_widgets(
    std::unordered_map<std::string, std::shared_ptr<Button>>& btns, 
    std::unordered_map<std::string, std::shared_ptr<Text>>& txts)
{
    // Buttons
    btns["Start"]  = Button::WithClass<DefaultButton>::Create("Start");
    btns["Option"] = Button::WithClass<DefaultButton>::Create("Option");
    btns["Resume"] = Button::WithClass<DefaultButton>::Create("Resume");
    btns["Close"]  = Button::WithClass<DefaultButton>::Create("Quit");
    btns["Moins"]  = Button::WithClass<SmallButton>::Create("-");
    btns["Plus"]   = Button::WithClass<SmallButton>::Create("+");
    btns["Apply"]  = Button::Create("Apply");

    // Texts
    txts["Title"]  = Text::WithClass<Title>::Create("The Game");
    txts["Pause"]  = Text::WithClass<Title>::Create("Game paused");
    txts["Option"] = Text::WithClass<Title>::Create("Options");
    txts["Lights"] = Text::Create("Lights");
    txts["Count"]  = Text::Create(std::to_string(m_lights_count));
    txts["Ig1"]    = Text::WithClass<Info>::Create("");
    txts["Ig2"]    = Text::WithClass<Info>::Create(Text::Block{
        "Press [escape] to pause"
    });
}

void UiCube::_create_layouts(
    std::unordered_map<std::string, std::shared_ptr<Button>>& btns, 
    std::unordered_map<std::string, std::shared_ptr<Text>>& txts)
{
    Scene& scene = Service<Window>::get().scene();

    // Start
    m_layouts["Start"] = Layout::Create<VerticalLayout>(scene);
    {
        m_layouts["Start"]->add(txts["Title"]);
        m_layouts["Start"]->add(btns["Start"]);
        m_layouts["Start"]->add(btns["Option"]);
        m_layouts["Start"]->add(btns["Close"]);
    }

    // Options
    m_layouts["Option"] = Layout::Create<VerticalLayout>(scene);
    {
        m_layouts["LLight"] = Layout::Create<HorizontalLayout>(scene);
        {
            m_layouts["LLight"]->add(txts["Lights"]);
            m_layouts["LLight"]->add(btns["Moins"]);
            m_layouts["LLight"]->add(txts["Count"], "#Count");  // Set an Id to retrieve it later
            m_layouts["LLight"]->add(btns["Plus"]);
        }

        m_layouts["Option"]->add(txts["Option"]);
        m_layouts["Option"]->add(m_layouts["LLight"]);
        m_layouts["Option"]->add(btns["Apply"]);
    }

    // In game
    m_layouts["InGame"] = Layout::Create<Layout>(scene);
    {
        m_layouts["InGame"]->add(txts["Ig1"], 0.01f, 0.04f, "#Ig1"); // Set an Id to retrieve it later
        m_layouts["InGame"]->add(txts["Ig2"], 0.01f, 0.12f);
    }

    // Pause
    m_layouts["Pause"] = Layout::Create<VerticalLayout>(scene);
    {
        m_layouts["Pause"]->add(txts["Pause"]);
        m_layouts["Pause"]->add(btns["Resume"]);
        m_layouts["Pause"]->add(btns["Option"]);
        m_layouts["Pause"]->add(btns["Close"]);
    }
}

