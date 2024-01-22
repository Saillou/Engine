#include "Scene.hpp"

#include <glad/glad.h>

// Instance
Scene::Scene(int widthHint, int heightHint):
    m_width(widthHint),
    m_height(heightHint),
    _framebuffer_main(Framebuffer::Multisample, widthHint, heightHint),
    _internalFrame(Framebuffer::Unique, widthHint, heightHint)
{
    _init_gl_config();
}

void Scene::clear() {
    // Cleanup previous draws
    glClearColor(0.05f, 0.05f, 0.06f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::run() {
    // Setup
    _update_camera();

    // Prepare
    Viewport(width(), height());
    clear();

    // Application draw
    _renderer._clear();
    _renderer.deferred = m_enable_deffered_draw;
    
    Event::Emit(SceneEvents::Draw());

    if (m_enable_deffered_draw) {
        // Resolve draw order and render shadow scene
        Viewport(_renderer._shadower.width(), _renderer._shadower.height());
        _renderer._compute();

        // Render main scene
        Viewport(width(), height());
        _framebuffer_main.bind();
        {
            _framebuffer_main.clear();

            _renderer.deferred = false;
            _renderer._draw();
        }
        _framebuffer_main.unbind();
    
        // Apply filters
        _framebuffer_main.texture().activate(GL_TEXTURE0);
        clear();
        
        Event::Emit(SceneEvents::PostDraw());
        drawFrame(_framebuffer_main);
        Event::Emit(SceneEvents::RenderFinished());
    }
}

void Scene::drawFrame(const Framebuffer& framebuffer) {
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

    // Need to blit multisample to mono
    if (framebuffer.type() == Framebuffer::Type::Multisample) {
        _internalFrame.resize(framebuffer.width(), framebuffer.height());
        Framebuffer::Blit(framebuffer, _internalFrame);
        _internalFrame.texture().bind();
    }
    else if (framebuffer.type() == Framebuffer::Type::Cubemap) {
        // ..
    }
    else {
        framebuffer.texture().bind();
    }

    _quad.draw();

    Texture::unbind(GL_TEXTURE_2D);

    glEnable(GL_DEPTH_TEST); // set back to original state.
}

void Scene::Viewport(int width, int height) {
    glViewport(0, 0, width, height);
}
void Scene::Viewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void Scene::resize(int width, int height) {
    m_width  = width;
    m_height = height;
    _framebuffer_main.resize(width, height);

    _update_camera();
    Event::Emit(SceneEvents::Resized(width, height));
}

void Scene::_init_gl_config() {
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
}

void Scene::_update_camera() {
    if (m_height == 0)
        return;

    float aspect = (float)m_width / m_height;

    _renderer._camera.lookAt(glm::vec3(0, 0, 1));
    _renderer._camera.usePerspective(aspect);
}

int Scene::width() const {
    return m_width;
}
int Scene::height() const {
    return m_height;
}
Renderer& Scene::renderer() {
    return _renderer;
}
Camera& Scene::camera() {
    return _renderer._camera;
}
std::vector<Light>& Scene::lights() {
    return _renderer._lights;
}

Framebuffer& Scene::framebuffer_main() {
    return _framebuffer_main;
}
