#include "BaseScene.hpp"

#include <glad/glad.h>

BaseScene::BaseScene(int widthHint, int heightHint):
    m_width(widthHint),
    m_height(heightHint),
    _internalFrame(Framebuffer::Unique, widthHint, heightHint)
{
    _init_gl_config();
}

BaseScene::~BaseScene() {
}

void BaseScene::clear() {
    // Cleanup previous draws
    glClearColor(0.05f, 0.05f, 0.06f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void BaseScene::run() {
    // Setup
    _update_camera();

    // Prepare
    Viewport(width(), height());
    clear();
    _renderer._clear();

    // Application draw
    _draw();

    // Resolve draw order and render shadow scene
    Viewport(_renderer._shadower.width(), _renderer._shadower.height());
    _renderer._compute();

    // Render main scene
    Viewport(width(), height());
    _renderer._draw();

    // Apply filters
    _post_draw();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BaseScene::_on_resize() {
    // to be overrided
}

void BaseScene::_draw() {
    // to be overrided
}

void BaseScene::_post_draw() {
    // to be overrided
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void BaseScene::drawFrame(const Framebuffer& framebuffer) {
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

    glEnable(GL_DEPTH_TEST); // set back to original state.
}

void BaseScene::Viewport(int width, int height) {
    glViewport(0, 0, width, height);
}
void BaseScene::Viewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void BaseScene::resize(int width, int height) {
    m_width  = width;
    m_height = height;

    _update_camera();
    _on_resize();
}

void BaseScene::_init_gl_config() {
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
}

void BaseScene::_update_camera() {
    if (m_height == 0)
        return;

    float aspect = (float)m_width / m_height;

    _renderer._camera.lookAt(glm::vec3(0, 0, 1));
    _renderer._camera.usePerspective(aspect);
}

int BaseScene::width() const {
    return m_width;
}
int BaseScene::height() const {
    return m_height;
}
Renderer& BaseScene::renderer() {
    return _renderer;
}
Camera& BaseScene::camera() {
    return _renderer._camera;
}
std::vector<Light>& BaseScene::lights() {
    return _renderer._lights;
}
