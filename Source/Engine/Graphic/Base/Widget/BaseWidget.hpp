#pragma once

#include <functional>
#include <vector>
#include <memory>

#include "WidgetEvents.hpp"
#include "../Scene.hpp"
#include "../../Wrapper/Framebuffer.hpp"
#include "../../../Events/CommonEvents.hpp"
#include "../../../Utils/Timer.hpp"

// -- Widget --
struct BaseWidget : 
    protected Event::Subscriber,
    protected Event::Emitter 
{
    BaseWidget();

    // Methods
    virtual void draw(Scene&) = 0;

    // Helper
    static glm::vec4 Transparent();

    // Members
    float opacity = 1.0f;

protected:
    virtual void _onClick();

private:
    void _on_mouse_button(const CommonEvents::MouseButton& evt);
};
