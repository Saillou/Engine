#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <Engine/Events/CommonEvents.hpp>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Utils/Filter/BaseFilter.hpp>

#include <Engine/Graphic/Wrapper/Framebuffer.hpp>
#include <Engine/Graphic/Base/Scene.hpp>


struct Ui : Event::Subscriber {
    Ui(Scene& scene);
    virtual ~Ui() = default;

protected:
    virtual void _on_resize(const SceneEvents::Resized&);
    virtual void _draw(const SceneEvents::RenderFinished&);

private:
    Scene& m_scene;

    Quad surface;
    Shader shader;
    Framebuffer frame_solo_a;
    Framebuffer frame_solo_b;
    Framebuffer frame_solo_c;
    Framebuffer m_frame;
};