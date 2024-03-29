#include "OverlaySystem.hpp"

#include "../../Graphic/Base/SceneFrame.hpp"

OverlaySystem::OverlaySystem(const Camera& camera) : 
    m_camera(camera)
{
    _canvas_system = ECS::registerSystem<CanvasSystem>(camera);
}

void OverlaySystem::init() 
{
    _canvas_system->init();
}
void OverlaySystem::update() 
{
    glDisable(GL_DEPTH_TEST);
    TextEngine::SetViewport(0, 0, (int)m_camera.screenSize.x, (int)m_camera.screenSize.y);

    _canvas_system->draw();

    // --- Todo: remove this and use entities instead --------
    // Current blocker: how to access the "draw" method ? 
    for (SceneFrame* frame : _frames) {
        frame->draw();
    }
    // --------------------------------------------------------

    glEnable(GL_DEPTH_TEST);
}

// - not good -
void OverlaySystem::add_frame(SceneFrame* frame) {
    _frames.insert(frame);
}
void OverlaySystem::remove_frame(SceneFrame* frame) {
    _frames.erase(frame);
}

// Drawing methods
void OverlaySystem::quad(const Quad& surface) 
{
    ShaderManager::Get(CookType::Surface)
        .use()
        .set("LocalModel", surface.absolute_dimensions ?
            glm::translate(glm::scale(glm::mat4(1.0f),
                glm::vec3(surface.w / 2.0f, surface.h / 2.0f, 0.0f)),
                glm::vec3(
                    +1.0f + surface.x / (surface.w / 2.0f),
                    -1.0f + (m_camera.screenSize.y - surface.y) / (surface.h / 2.0f),
                    0)) : surface.transform())
        .set("Projection", surface.absolute_dimensions ?
            glm::ortho(0.0f, m_camera.screenSize.x, 0.0f, m_camera.screenSize.y) :
            glm::mat4(1.0f))
        .set("quadTexture", surface.texture_location)
        .set("background_color", surface.diffuse_color);

    surface.drawElements();
}

void OverlaySystem::text(const std::string& text, float x, float y, float scale, const glm::vec4& color) 
{
    TextEngine::Write(text, x, y, scale, color);
}