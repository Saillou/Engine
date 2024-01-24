#pragma once

#include <Engine/Events/Events.hpp>
#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Base/Widget/BaseLayout.hpp>

struct Ui : Event::Subscriber {
    Ui(Scene& scene);
    virtual ~Ui() = default;

    bool visible = true;

protected:
    void draw_sp(const BaseLayout* emitter, const LayoutEvents::Draw& msg);

private:
    void _cleanup();

    Scene& m_scene;
    BaseLayout m_layout;
};