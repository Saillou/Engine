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
    friend struct BaseLayout;

    BaseWidget();

    // Methods
    virtual void draw(Scene&) = 0;

    // Helper
    static bool IsIn(const BaseWidget&, int x, int y);
    static glm::vec4 Transparent();

    // Access
    const BaseLayout* parent() const;

    // Members
    float opacity = 1.0f;

protected:
    // Used to know draw reference
    BaseLayout* _parent = nullptr;

    // Used for draw and mouse hitbox
    std::vector<std::unique_ptr<Quad>> _surfaces;

private:
    void _on_mouse_button(const CommonEvents::MouseButton& evt);
};
