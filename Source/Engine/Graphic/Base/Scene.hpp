#pragma once

#include "SceneEvents.hpp"
#include "../Wrapper/Framebuffer.hpp"
#include "../../Framework/Core/ECS.hpp"
#include "../../Framework/System/CasterSystem.hpp"
#include "../../Framework/System/RenderSystem.hpp"
#include "../../Framework/System/OverlaySystem.hpp"
#include "../../Framework/System/ColliderSystem.hpp"

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
    //void drawFrame(const Framebuffer& framebuffer);

    RenderSystem&   renderer();
    OverlaySystem&  overlayer();
    CasterSystem&   raycaster();
    ColliderSystem& collider();

    // getters
    int width()  const;
    int height() const;

    Camera camera             = {}; // TODO: remove from here and define as components (?)
    std::vector<Light> lights = {}; // TODO: remove from here and define as components (?)
    //Framebuffer& framebuffer_main();

private:
    void _init_gl_config();
    void _update_camera();

    // Members
    int m_width  = 0;
    int m_height = 0;

    //Framebuffer _internalFrame;     // Used when drawing multisample frame
    //Framebuffer _framebuffer_main;
    //Quad _quad;

    // Default systems
    std::shared_ptr<RenderSystem>   m_render_system;
    std::shared_ptr<OverlaySystem>  m_overlay_system;
    std::shared_ptr<CasterSystem>   m_caster_system;
    std::shared_ptr<ColliderSystem> m_collider_system;
};