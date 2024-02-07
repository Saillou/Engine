#include "WorldEditor.h"

#include <utility>

WorldEditor::WorldEditor(Scene& scene) : 
    Editor(scene)
{ }

void WorldEditor::onEnter()
{
    // Set menu
    m_menu.reset();

    // Set Scene
    m_scene.lights() = {};

    m_scene.camera().position  = glm::vec3(0.0f, -5.0f, 0.15f);
    m_scene.camera().direction = glm::vec3(0, 0, 0);

    // Create quad with texture
    m_entities["quad"] = Entity(Entity::SimpleShape::Quad);
    m_entities["quad"].model().root()->meshes.front()->textures().push_back(
        TextureData{"texture_diffuse", std::make_unique<Texture>("Resources/textures/grass.png")}
    );
    m_entities["quad"].localPose()     = glm::rotate(glm::mat4(1.0f), glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));
    m_entities["quad"].localMaterial() = Material{ glm::vec4(1, 0, 0, 1), false };
    m_entities["quad"].poses() = {
        glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)), glm::vec3(0.f, 0.f, 0.f)),
    };
}

void WorldEditor::onExit()
{ }

void WorldEditor::onUpdate()
{
    _drawScene();
    _drawMenu();
}

// - Private -
void WorldEditor::_drawMenu() {
    m_menu.show();
}

void WorldEditor::_drawScene() {
    Renderer& render = m_scene.renderer();

    for (auto& entity : m_entities) {
        render.draw(Render::DrawType::Lights, entity.second);
    }
}