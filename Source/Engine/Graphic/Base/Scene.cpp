#include "Scene.hpp"
#include "../TextEngine.hpp"

#include <glad/glad.h>

// Instance
Scene::Scene(int widthHint, int heightHint):
    m_width(widthHint),
    m_height(heightHint)
{
    _init_gl_config();

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

    Viewport(width(), height());

    // Start
    Event::Emit(SceneEvents::RenderStarted());

    // - Draw 3D-Scene
    m_render_system->update();
     
    // - Apply filters
    Event::Emit(SceneEvents::PostDraw());

    // - Draw UI
    m_overlay_system->update();

    // End
    Event::Emit(SceneEvents::RenderFinished());
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