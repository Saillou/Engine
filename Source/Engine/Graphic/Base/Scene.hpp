#pragma once

//#include "Render/Renderer.hpp"
#include "SceneEvents.hpp"
#include "../Wrapper/Framebuffer.hpp"
#include "../../Framework/Core/ECS.hpp"
#include "../../Framework/System/RenderSystem.hpp"

// Scene model
struct Scene {
    friend struct Window;

    Scene(int widthHint = 0, int heightHint = 0);

    static void clear();
    void run();                       
    void resize(int width, int height);

    static void Viewport(int width, int height);
    static void Viewport(int x, int y, int width, int height);

    // utils
    void drawFrame(const Framebuffer& framebuffer);

    // setters
    //void directDraw(bool);

    // getters
    int width() const;
    int height() const;

    Camera camera             = {}; // TODO: remove from here and define as components (?)
    std::vector<Light> lights = {}; // TODO: remove from here and define as components (?)
    //Renderer& renderer();
    Framebuffer& framebuffer_main();

private:
    void _init_gl_config();
    void _update_camera();

    // Members
    //int m_enable_deffered_draw = true; // Disable it to manually manage draw pipeline
    int m_width  = 0;
    int m_height = 0;
    std::shared_ptr<RenderSystem> m_render_system;

    //Renderer _renderer;
    Framebuffer _internalFrame;     // Used when drawing multisample frame
    Framebuffer _framebuffer_main;
    Quad _quad;
};