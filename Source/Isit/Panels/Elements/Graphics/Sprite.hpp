#pragma once

#include <Engine/Framework/Service.hpp>
#include <Engine/Events/Events.hpp>
#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Window.hpp>
#include <Engine/Graphic/Base/Widget/Text.hpp>
#include <Engine/Utils/Animator.hpp>

#include "CanvasEntity.hpp"

struct Sprite : Event::Subscriber {
    Sprite();
    virtual ~Sprite() = default;

    virtual void draw() = 0;
    virtual void clear();

protected:
    virtual void onMouseOver()      {};
    virtual void onMouseOut()       {};
    virtual void onMouseReleased()  {};
    virtual void onMousePressed()   {};

    virtual bool _hitArea(int x, int y) = 0;

    const Scene& _scene;
    CanvasEntity _canvas;

private:
    bool __is_mouse_over = false;
};