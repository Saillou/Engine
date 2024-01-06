#pragma once

#include "../TextEngine.hpp"
#include "../Camera.hpp"
#include "../Utils/Framebuffer.hpp"

#include "../Light.hpp"
#include "../Cookable.hpp"
#include "Shapes/Quad.hpp"

// Scene model
struct BaseScene {
    BaseScene(int widthHint = 0, int heightHint = 0);
    virtual ~BaseScene();

    virtual void clear();
    virtual void draw();                            // called by an instance of a `Window::update()`
    virtual void resize(int width, int height);     // called by an event `Window::resize()`

    static void Viewport(int width, int height);
    static void Viewport(int x, int y, int width, int height);

    // utils
    void drawFrame(Framebuffer& framebuffer);

    // getters
    int width() const;
    int height() const;
    Camera& camera();
    std::vector<Light>& lights();

protected:
    virtual void _init_gl_config(); // Override this to enable or disable some opengl functionalities. (eg PENCIL_TEST)
    virtual void _update_camera();  // Override this if camera perspective isn't the default one.
    virtual void _onResize();       // Override this to resize framebuffer or texture depending on the window size.

    // Members
    Camera m_camera = {};
    std::vector<Light> m_lights;

    int m_width  = 0;
    int m_height = 0;

private:
    Framebuffer _internalFrame;     // Used when drawing multisample frame
    Quad _quad;
};