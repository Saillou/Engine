#pragma once

#include <Engine/Events/Events.hpp>
#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Base/Widget/BaseLayout.hpp>
#include <Engine/Graphic/Base/Widget/ButtonWidget.hpp>
#include <Engine/Graphic/Base/Widget/TextWidget.hpp>

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

private:
    Scene& m_scene;

    State m_state;
    std::shared_ptr<BaseLayout> m_layout;
    std::unordered_map<std::string, std::shared_ptr<ButtonWidget>> m_buttons;
    std::unordered_map<std::string, std::shared_ptr<TextWidget>> m_texts;
    int m_count = 2;
};