#pragma once

#include "Layout.hpp"

struct MainLayout : 
    public Layout
{
    // -- Layout --
    MainLayout(Scene& scene);
    virtual ~MainLayout() = default;

private:
    void _on_draw(const SceneEvents::RenderFinished&);

    Framebuffer m_frame;
    CopyFilter m_copyFilter;
};
