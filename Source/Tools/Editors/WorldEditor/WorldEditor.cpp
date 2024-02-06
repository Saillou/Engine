#include "WorldEditor.h"

WorldEditor::WorldEditor(Scene& scene) : 
    Editor(scene)
{ }

void WorldEditor::onEnter()
{
    // Set menu
    m_menu.reset();

    // Set lights
    m_scene.lights() = {
        { glm::vec3{ 0, -0.5f, 0.5f }, glm::vec4{ 1,1,1,1 } }
    };

    // Camera
    m_scene.camera().position  = glm::vec3(0.0f, -2.0f, 0.15f);
    m_scene.camera().direction = glm::vec3(0, 0, 0);

    // Create entities
    m_entities["sphere"] = Entity(Entity::SimpleShape::Sphere);
    m_entities["sphere"].setPosesWithMaterials(
        { glm::scale(glm::mat4(1.0f), glm::vec3(0.15f)) },
        { Material{glm::vec4(1, 0, 0, 1), false} }
    );
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

    render.draw(Render::DrawType::Lights, m_entities["sphere"]);
}