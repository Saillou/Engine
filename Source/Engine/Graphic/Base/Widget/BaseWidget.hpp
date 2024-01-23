#pragma once

#include <functional>

#include "../Scene.hpp"
#include "../../Wrapper/Framebuffer.hpp"
#include "../../../Events/CommonEvents.hpp"
#include "../../../Utils/Filter/CopyFilter.hpp"
#include "../../../Utils/Timer.hpp"

struct BaseWidget : Event::Subscriber {
    BaseWidget(Scene& scene);
    virtual ~BaseWidget() = default;

    // Setters
    Scene& scene();

    // Callbacks
    std::function<void(void)>& on_draw();

    // Members
    float opacity = 1.0f;

protected:
    virtual void _on_resize(const SceneEvents::Resized&);
    virtual void _draw(const SceneEvents::RenderFinished&);

private:
    Scene& m_scene;
    Framebuffer m_frame;
    CopyFilter m_copyFilter;
    std::function<void(void)> m_callback_draw;
};
