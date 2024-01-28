#include "Ui.hpp"

// Classnames
static constexpr char Title[]         = ".Title";
static constexpr char SmallButton[]   = ".SmallButton";
static constexpr char DefaultButton[] = ".DefaultButton";

// -- Ui --
Ui::Ui(Scene& scene) : 
    m_scene(scene),
    m_prev_state(Ui::State::Idle),
    m_state(Ui::State::Idle),
    m_main_frame(scene)
{
    // Temporary container for gui elements
    std::unordered_map<std::string, std::shared_ptr<Button>> btns;
    std::unordered_map<std::string, std::shared_ptr<Text>>   txts;

    // Create layouts
    m_main_frame.layout().styleSheet() = _create_style();
    _create_widgets(btns, txts);
    _create_layouts(btns, txts);
    
    // Define events
    _subscribe(&m_main_frame, &Ui::draw);

    _subscribe(btns.at("Start"),  [=](const CommonEvents::MouseButton&) { setState(State::InGame); });
    _subscribe(btns.at("Resume"), [=](const CommonEvents::MouseButton&) { setState(State::InGame); });
    _subscribe(btns.at("Option"), [=](const CommonEvents::MouseButton&) { setState(State::Option); });
    _subscribe(btns.at("Apply"),  [=](const CommonEvents::MouseButton&) { setState(m_prev_state);  });
    _subscribe(btns.at("Moins"),  [=](const CommonEvents::MouseButton&) { _updateCount(-1);        });
    _subscribe(btns.at("Plus"),   [=](const CommonEvents::MouseButton&) { _updateCount(+1);        });
    _subscribe(btns.at("Close"),  [=](const CommonEvents::MouseButton&) { m_wantQuit = true;       });

    // Let's start
    setState(Ui::State::Option);
}

void Ui::setState(Ui::State state) {
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
        m_main_frame.layout().style().set_opacity(1.0f);
        m_main_frame.layout().style().set_background(Style::Transparent());

        m_main_frame.layout().add(m_layouts["InGame"]);
        break;

    case State::Pause:
        m_main_frame.layout().add(m_layouts["Pause"]);
        break;
    }

    // Notify it changed
    Event::Emit(CommonEvents::StateUpdated(), this);
}

Ui::State Ui::state() const {
    return m_state;
}

int Ui::lightsCount() const {
    return m_lights_count;
}

bool Ui::wantQuit() const {
    return m_wantQuit;
}

void Ui::draw(const LayoutEvents::Draw& msg) {
    switch (m_state) {
    case State::InGame:
        m_layouts["InGame"]->find<Text>("#Ig1")->at(0) = "Cam pos: " + glm::to_string(m_scene.camera().position);
        m_layouts["InGame"]->find<Text>("#Ig1")->at(1) = "Cam dir: " + glm::to_string(m_scene.camera().direction);
        break;
    }
}

void Ui::_updateCount(int delta) {
    if (m_lights_count + delta < 0 || m_lights_count + delta > 5)
        return;

    m_lights_count += delta;
    m_layouts["LLight"]->find<Text>("#Count")->setText(std::to_string(m_lights_count));
}

// ---------- Helpers ----------
StyleSheet Ui::_create_style() const
{
    return StyleSheet::CreateDefault()
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
}

void Ui::_create_widgets(
    std::unordered_map<std::string, std::shared_ptr<Button>>& btns, 
    std::unordered_map<std::string, std::shared_ptr<Text>>& txts)
{
    // Buttons
    btns["Start"]  = Widget::WithClass<DefaultButton>::Create<Button>("Start");
    btns["Option"] = Widget::WithClass<DefaultButton>::Create<Button>("Option");
    btns["Resume"] = Widget::WithClass<DefaultButton>::Create<Button>("Resume");
    btns["Close"]  = Widget::WithClass<DefaultButton>::Create<Button>("Quit");
    btns["Moins"]  = Widget::WithClass<SmallButton>::Create<Button>("-");
    btns["Plus"]   = Widget::WithClass<SmallButton>::Create<Button>("+");
    btns["Apply"]  = Widget::Create<Button>("Apply");

     //Texts
    txts["Title"]  = Widget::WithClass<Title>::Create<Text>("The Game");
    txts["Pause"]  = Widget::WithClass<Title>::Create<Text>("Game paused");
    txts["Option"] = Widget::WithClass<Title>::Create<Text>("Options");
    txts["Lights"] = Widget::Create<Text>("Lights");
    txts["Count"]  = Widget::Create<Text>(std::to_string(m_lights_count));
    txts["Ig1"]    = Widget::Create<Text>("");
    txts["Ig2"]    = Widget::Create<Text>(Text::Block{
        "Press [space] to pause",
        "Press [R] to dis/enable filters",
        "Press [T] to dis/enable casters"
    });
}

void Ui::_create_layouts(
    std::unordered_map<std::string, std::shared_ptr<Button>>& btns, 
    std::unordered_map<std::string, std::shared_ptr<Text>>& txts)
{
    // Start
    m_layouts["Start"] = Widget::Create<VerticalLayout>(m_scene);
    {
        m_layouts["Start"]->add(txts["Title"]);
        m_layouts["Start"]->add(btns["Start"]);
        m_layouts["Start"]->add(btns["Option"]);
        m_layouts["Start"]->add(btns["Close"]);
    }

    // Options
    m_layouts["Option"] = Widget::Create<VerticalLayout>(m_scene);
    {
        m_layouts["LLight"] = Widget::Create<HorizontalLayout>(m_scene);
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
    m_layouts["InGame"] = Widget::Create<Layout>(m_scene);
    {
        m_layouts["InGame"]->add(txts["Ig1"], "#Ig1");          // Set an Id to retrieve it later
        m_layouts["InGame"]->add(txts["Ig2"]);
    }

    // Pause
    m_layouts["Pause"] = Widget::Create<VerticalLayout>(m_scene);
    {
        m_layouts["Pause"]->add(btns["Resume"]);
        m_layouts["Pause"]->add(btns["Option"]);
        m_layouts["Pause"]->add(txts["Pause"]);
        m_layouts["Pause"]->add(btns["Close"]);
    }
}

