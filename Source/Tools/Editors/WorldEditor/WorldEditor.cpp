#include "WorldEditor.hpp"

// -------- Editor --------
WorldEditor::WorldEditor(Scene& scene) : 
    Editor(scene)
{ }

void WorldEditor::onEnter() {
    m_menu.reset();

    m_scene.lights = { };
    m_scene.camera.position  = glm::vec3(0, -30, 0);
    m_scene.camera.direction = glm::vec3(0, 0, 0);

    m_entities["cube1"] = ManagedEntity::Create(Model::Load(Model::SimpleShape::Cube));
    m_entities["cube1"]->local() = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    m_entities["cube1"]->color() = glm::vec4(1.0f, 0, 0, 1.0f);

    m_entities["cube2"] = ManagedEntity::Create(Model::Load(Model::SimpleShape::Cube));
    m_entities["cube2"]->local() = glm::translate(glm::mat4(1.0f), glm::vec3(2, 0, 0));
    m_entities["cube2"]->color() = glm::vec4(0, 1.0f, 0, 1.0f);
}

void WorldEditor::onExit() { 
    m_entities.clear();
}

void WorldEditor::onUpdate() {
    _drawScene();
    _drawInfo();
}

// - Private -
void WorldEditor::_drawScene() {
    // .. 
}

void WorldEditor::_drawInfo() {
    m_menu.createSection("Camera");
    m_menu.addContent("Camera", "position",  m_scene.camera.position);
    m_menu.addContent("Camera", "direction", m_scene.camera.direction);

    m_menu.createSection("Lights");
    m_menu.addContent("Lights", "-", "-");

    m_menu.show();
}
