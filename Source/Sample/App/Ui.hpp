#pragma once

#include <Engine/Events/Events.hpp>
#include <Engine/Graphic/Base/Widget/Layout/MainLayout.hpp>

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

    State m_state, 
          m_prev_state;
    int m_lights_count = 2;

    Scene& m_scene;
    std::unordered_map<std::string, std::shared_ptr<Layout>> m_layouts;
};
