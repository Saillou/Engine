#pragma once

#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Base/Widget/BaseWidget.hpp>

struct Ui : Event::Subscriber {
    Ui(Scene& scene);
    virtual ~Ui() = default;

    bool visible = true;

protected:
    void draw();

private:
    void _cleanup();

    Scene& m_scene;
    BaseWidget m_widget;
    Shader shad_quad;
};