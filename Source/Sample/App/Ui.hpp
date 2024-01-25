#pragma once

#include <Engine/Events/Events.hpp>
#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Base/Widget/BaseLayout.hpp>
#include <Engine/Graphic/Base/Widget/ButtonWidget.hpp>

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

protected:
    void draw(const LayoutEvents::Draw& msg);
    void onClickStart(const CommonEvents::MouseButton& msg);
    void onClickOption(const CommonEvents::MouseButton& msg);

private:
    Scene& m_scene;

    State m_state;
    std::shared_ptr<BaseLayout> m_layout;
    std::unordered_map<std::string, std::shared_ptr<BaseWidget>> m_widgets;
    int m_count = 0;
};