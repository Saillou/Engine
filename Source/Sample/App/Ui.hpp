#pragma once

#include <Engine/Events/Events.hpp>
#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Base/Widget/BaseLayout.hpp>

struct Ui : Event::Subscriber {
    Ui(Scene& scene);
    virtual ~Ui() = default;

    enum class State {
        Start,
        InGame
    };
    
    void setState(Ui::State state);

protected:
    void draw(const BaseLayout* emitter, const LayoutEvents::Draw& msg);

private:

    Scene& m_scene;
    BaseLayout m_layout;
    Ui::State m_state;
};