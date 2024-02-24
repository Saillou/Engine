#include "WorldEditor.hpp"
#include "WorldEditorHelper.hpp"


// Helper functions
using namespace WorldEditorHelper;

// -------- Editor --------
WorldEditor::WorldEditor(Scene& scene) : 
    Editor(scene)
{ }

void WorldEditor::onEnter() {
    m_menu.reset();

    m_scene.lights = { };
    m_scene.camera.position = glm::vec3(0, -4.0f, 0);
    m_scene.camera.direction = glm::vec3(0, 0, 0);

    // ..
}

void WorldEditor::onExit() { 
    // ..
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
