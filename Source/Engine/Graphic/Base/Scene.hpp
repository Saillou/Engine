#pragma once

#include "Render/Renderer.hpp"
#include "../../Events/Events.hpp"

// Define events available
struct SceneEvents : public Event {
    enum _Type {
        Draw_ = 1024,
        PostDraw_,
        Resized_,
        RenderFinished_,
    };

    // -- Inputs --
    struct Draw : public _Base {
        Draw():
            _Base(_Type::Draw_)
        {};
    };
    struct PostDraw : public _Base {
        PostDraw():
            _Base(_Type::PostDraw_)
        {};
    };
    struct Resized : public _Base {
        Resized(int width = 0, int height = 0):
            _Base(_Type::Resized_), width(width), height(height)
        {};

        int width;
        int height;
    };
    struct RenderFinished : public _Base {
        RenderFinished() :
            _Base(_Type::RenderFinished_)
        {};
    };
};

// Scene model
struct Scene {
    friend struct Window;

    Scene(int widthHint = 0, int heightHint = 0);

    void clear();
    void run();                       
    void resize(int width, int height);

    static void Viewport(int width, int height);
    static void Viewport(int x, int y, int width, int height);

    // utils
    void drawFrame(const Framebuffer& framebuffer);

    // getters
    int width() const;
    int height() const;

    Renderer& renderer();
    Camera& camera();
    std::vector<Light>& lights();
    Framebuffer& framebuffer_main();

private:
    void _init_gl_config();
    void _update_camera();

    // Members
    int m_enable_deffered_draw = true; // Disable it to manually manage draw pipeline
    int m_width  = 0;
    int m_height = 0;

    Renderer _renderer;
    Framebuffer _internalFrame;     // Used when drawing multisample frame
    Framebuffer _framebuffer_main;
    Quad _quad;
};