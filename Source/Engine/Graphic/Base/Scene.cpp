#include "Scene.hpp"
#include "../TextEngine.hpp"

#include <glad/glad.h>

// Instance
Scene::Scene(int widthHint, int heightHint):
    m_width(widthHint),
    m_height(heightHint)
    //_framebuffer_main(Framebuffer::Multisample, widthHint, heightHint),
    //_internalFrame(Framebuffer::Unique, widthHint, heightHint)
{
    _init_gl_config();
    //_quad.mesh->setupVao();

    // Init default system
    m_render_system   = ECS::registerSystem<RenderSystem>(camera, lights);
    m_overlay_system  = ECS::registerSystem<OverlaySystem>(camera);
    m_caster_system   = ECS::registerSystem<CasterSystem>(camera);
    m_collider_system = ECS::registerSystem<ColliderSystem>();

    m_render_system->init();
    m_overlay_system->init();
    m_caster_system->init();
    m_collider_system->init();
}

void Scene::clear() {
    // Cleanup previous draws
    glClearColor(0.05f, 0.05f, 0.06f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::run() {
    // Setup
    _update_camera();
    clear();

    // Start
    Viewport(width(), height());
    Event::Emit(SceneEvents::RenderStarted());

    // - Draw 3D-Scene
    //_framebuffer_main.bind();
    //{
        //_framebuffer_main.clear();
        m_render_system->update();
    //}
    //_framebuffer_main.unbind();
     

    // - Apply filters
    Event::Emit(SceneEvents::PostDraw());

    // - Draw UI
    //_framebuffer_main.bind();
    //{
        //_framebuffer_main.clear();
        m_overlay_system->update();
    //}
    //_framebuffer_main.unbind();

    // End
    Event::Emit(SceneEvents::RenderFinished());
}

//void Scene::drawFrame(const Framebuffer& framebuffer) {
//    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
//    Texture::activate(GL_TEXTURE0);
//
//    // Need to blit multisample to mono
//    if (framebuffer.type() == Framebuffer::Type::Multisample) {
//        _internalFrame.resize(framebuffer.width(), framebuffer.height());
//        Framebuffer::Blit(framebuffer, _internalFrame);
//        _internalFrame.texture().bind();
//    }
//    else if (framebuffer.type() == Framebuffer::Type::Cubemap) {
//        // ..
//    }
//    else {
//        framebuffer.texture().bind();
//    }
//    
//    // Draw
//    _quad.texture_location = 0;
//    //_renderer.quad(_quad); // TODO: Don't forget me
//
//    // set back to original state.
//    Texture::deactivate(GL_TEXTURE_2D, GL_TEXTURE0);
//    glEnable(GL_DEPTH_TEST); 
//}

void Scene::Viewport(int width, int height) {
    glViewport(0, 0, width, height);
}
void Scene::Viewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void Scene::resize(int width, int height) {
    m_width  = width;
    m_height = height;

    TextEngine::SetViewport(0, 0, width, height);
    //_framebuffer_main.resize(width, height);

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
    camera.screenSize = glm::vec2(width(), height());

    if (m_height == 0)
        return;
    float aspect = (float)m_width / m_height;

    camera.lookAt();
    camera.usePerspective(aspect);
}

int Scene::width() const {
    return m_width;
}
int Scene::height() const {
    return m_height;
}
//
//Framebuffer& Scene::framebuffer_main() {
//    return _framebuffer_main;
//}

// System direct access
RenderSystem& Scene::renderer() {
    return *m_render_system;
}
OverlaySystem& Scene::overlayer() {
    return *m_overlay_system;
}
CasterSystem& Scene::raycaster() {
    return *m_caster_system;
}
ColliderSystem& Scene::collider() {
    return *m_collider_system;
}