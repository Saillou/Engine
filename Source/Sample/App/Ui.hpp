#pragma once

#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Base/Widget/BaseWidget.hpp>

struct Ui : Event::Subscriber {
    Ui(Scene& scene);
    virtual ~Ui() = default;

protected:
    void draw();

private:
    Scene& m_scene;
    BaseWidget m_widget;
};