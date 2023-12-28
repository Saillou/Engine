#include "BaseScene.hpp"

#include <glad/glad.h>

BaseScene::BaseScene():
    _internalFrame(Framebuffer::Unique)
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

void BaseScene::draw() {
    // to be overrided
}

void BaseScene::_onResize() {
    // to be overrided
}

void BaseScene::drawFrame(Framebuffer& framebuffer) {
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

    // Need to blit multisample to mono
    if (framebuffer.type() == Framebuffer::Type::Multisample) {
        _internalFrame.resize(framebuffer.width(), framebuffer.height());
        Framebuffer::Blit(framebuffer, _internalFrame);
        _internalFrame.texture().bind();
    }
    else {
        framebuffer.texture().bind();
    }

    _quad.draw();

    glEnable(GL_DEPTH_TEST); // set back to original state.
}

void BaseScene::resize(int width, int height) {
    m_width = width;
    m_height = height;

    _update_camera();

    _onResize();
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

    m_camera.lookAt(glm::vec3(0, 0, 1));
    m_camera.usePerspective(aspect);
}

int BaseScene::width() const {
    return m_width;
}
int BaseScene::height() const {
    return m_height;
}
Camera& BaseScene::camera() {
    return m_camera;
}
std::vector<Light>& BaseScene::lights() {
    return m_lights;
}

