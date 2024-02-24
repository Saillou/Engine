#include "OverlaySystem.hpp"

OverlaySystem::OverlaySystem(const Camera& camera) : 
    m_camera(camera)
{

}

void OverlaySystem::init() 
{
    // System requirements
    // ..
    
    // Shaders
    addRecipe(Cookable::CookType::Shape);
}
void OverlaySystem::update() 
{
    TextEngine::SetViewport(0, 0, (int)m_camera.screenSize.x, (int)m_camera.screenSize.y);

    // TODO: to do
}

// 
void OverlaySystem::quad(const Quad& surface) 
{
    Shader& sh = get(Cookable::CookType::Shape)->use()
        .set("LocalModel", surface.absolute_dimensions ?
            glm::translate(glm::scale(glm::mat4(1.0f),
                glm::vec3(surface.w / 2.0f, surface.h / 2.0f, 0.0f)),
                glm::vec3(
                    +1.0f + surface.x / (surface.w / 2.0f),
                    -1.0f + (m_camera.screenSize.y - surface.y) / (surface.h / 2.0f),
                    0)) : surface.transform())
        .set("projection", surface.absolute_dimensions ?
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