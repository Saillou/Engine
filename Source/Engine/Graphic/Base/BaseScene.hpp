#pragma once

#include "Render/Renderer.hpp"

// Scene model
struct BaseScene {
    friend struct Window;

    BaseScene(int widthHint = 0, int heightHint = 0);
    virtual ~BaseScene();

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

protected:
    virtual void _init_gl_config(); // Override this to enable or disable some opengl functionalities. (eg PENCIL_TEST)
    virtual void _update_camera();  // Override this if camera perspective isn't the default one.
    virtual void _on_resize();      // Override this to resize framebuffer or texture depending on the window size.

    virtual void _draw();           // called at the rendering start
    virtual void _post_draw();      // called after `draw` method

    // Members
    int m_width  = 0;
    int m_height = 0;


private:
    Renderer _renderer;
    Framebuffer _internalFrame;     // Used when drawing multisample frame
    Quad _quad;
};