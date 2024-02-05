#pragma once

#include <glm/gtx/string_cast.hpp>

#include <Engine/Events/Events.hpp>
#include <Engine/Graphic/Base/SceneFrame.hpp>
#include <Engine/Graphic/Base/Widget/Text.hpp>
#include <Engine/Graphic/Base/Widget/Button.hpp>
#include <Engine/Graphic/Base/Widget/Layout/VerticalLayout.hpp>
#include <Engine/Graphic/Base/Widget/Layout/HorizontalLayout.hpp>


struct Ui : Event::Subscriber {
    Ui();
    virtual ~Ui() = default;

    enum class State {
        Idle, 
        Start, Option, InGame, Pause
    };
    
    // Accessors
    void setState(Ui::State state);

    State state()       const;
    int   lightsCount() const;
    bool  wantQuit()    const;

protected:
    void draw(const WidgetEvents::Draw& msg);

private:
    // - Methods
    void _updateCount(int delta);

    // - Members
    State m_state, 
          m_prev_state;
    int   m_lights_count = 2;
    bool  m_wantQuit = false;

    SceneFrame m_main_frame;
    std::unordered_map<std::string, std::shared_ptr<Layout>> m_layouts;

    // - Helpers -
    void _create_widgets(
        std::unordered_map<std::string, std::shared_ptr<Button>>& btns,
        std::unordered_map<std::string, std::shared_ptr<Text>>& txts
    );
    void _create_layouts(
        std::unordered_map<std::string, std::shared_ptr<Button>>& btns,
        std::unordered_map<std::string, std::shared_ptr<Text>>& txts
    );
    StyleSheet _create_style() const;
};
