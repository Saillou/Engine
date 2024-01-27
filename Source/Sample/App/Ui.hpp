#pragma once

#include <Engine/Events/Events.hpp>
#include <Engine/Graphic/Base/Scene.hpp>

#include <Engine/Graphic/Base/Widget/Layout.hpp>
#include <Engine/Graphic/Base/Widget/Button.hpp>
#include <Engine/Graphic/Base/Widget/Text.hpp>

struct Ui : Event::Subscriber {
    Ui(Scene& scene);
    virtual ~Ui() = default;

    enum class State {
        Idle,
        Start, 
        Option,
        InGame,
        Pause
    };
    
    // Accessors
    void setState(Ui::State state);

    State state() const;
    bool wantQuit = false;
    int getLightsCount() const;

protected:
    void draw(const LayoutEvents::Draw& msg);

private:
    void _updateCount(int delta);

    Scene& m_scene;

    State m_state;
    State m_prev_state;

    std::shared_ptr<Layout> m_layout;
    std::unordered_map<std::string, std::shared_ptr<Button>> m_buttons;
    std::unordered_map<std::string, std::shared_ptr<Text>>   m_texts;
    int m_lights_count = 2;
};
