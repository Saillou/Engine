#pragma once

#include "../Camera.hpp"
#include "../Cookable.hpp"
#include "../Light.hpp"
#include "../TextEngine.hpp"
#include "../Utils/Framebuffer.hpp"
#include "../../Utils/Objects/ShadowRender.hpp"

// Scene model
struct BaseScene {
    friend struct Window;

    BaseScene(int widthHint = 0, int heightHint = 0);
    virtual ~BaseScene();

    void clear();
    void draw();                       
    void resize(int width, int height);

    static void Viewport(int width, int height);
    static void Viewport(int x, int y, int width, int height);

    // utils
    void drawFrame(const Framebuffer& framebuffer);

    // getters
    int width() const;
    int height() const;
    Camera& camera();
    std::vector<Light>& lights();
    const ShadowRender* shadower();

protected:
    virtual void _init_gl_config(); // Override this to enable or disable some opengl functionalities. (eg PENCIL_TEST)
    virtual void _update_camera();  // Override this if camera perspective isn't the default one.
    virtual void _on_resize();      // Override this to resize framebuffer or texture depending on the window size.

    virtual void _draw_shadow(Shader& sh); // called by an instance of a `ShadowRender`
    virtual void _prepare_draw();          // called before `draw` method
    virtual void _draw();                  // called during `draw` method

    // Members
    Camera m_camera = {};
    std::vector<Light> m_lights;

    int m_width  = 0;
    int m_height = 0;


private:
    ShadowRender _shadowRender;    // Used to draw shadows
    Framebuffer _internalFrame;     // Used when drawing multisample frame
    Quad _quad;
};