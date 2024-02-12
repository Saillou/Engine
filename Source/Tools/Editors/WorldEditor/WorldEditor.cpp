#include "WorldEditor.hpp"
#include "WorldEditorHelper.hpp"


// Helper functions
using namespace WorldEditorHelper;

// -------- Editor --------
WorldEditor::WorldEditor(Scene& scene) : 
    Editor(scene)
{ }

void WorldEditor::onEnter() {
    // Change app draw style to be able to reorder entities and compute shadows
    m_scene.directDraw(false);

    m_menu.reset();
    m_scene.lights() = { };
    m_scene.camera().position = glm::vec3(0, -4.0f, 0);
    m_scene.camera().direction = glm::vec3(0, 0, 0);

    // Create entities
    m_models["grass"] = tile_with_texture("Resources/textures/grass.png");
    m_models["grass"]->poses.push_back(pose_scale(glm::vec2(0.0f, 0.0f), 1.0f));
}

void WorldEditor::onExit() { 
    // Put back original app state
    m_scene.directDraw(true);

    m_models.clear();

    // Disable events
    _unsubscribeAll();
}

void WorldEditor::onUpdate() {
    _drawScene();
    _drawInfo();
}

// - Private -
void WorldEditor::_drawScene() {
    // Draw items
    Renderer& render = m_scene.renderer();

    for (auto& model : m_models) {
        render.draw(Render::DrawType::Lights, model.second);
    }
}

void WorldEditor::_drawInfo() {
    m_menu.createSection("Camera");
    m_menu.addContent("Camera", "position",  m_scene.camera().position);
    m_menu.addContent("Camera", "direction", m_scene.camera().direction);

    m_menu.createSection("Lights");
    m_menu.addContent("Lights", "-", "-");

    m_menu.createSection("Entities");
    m_menu.addContent("Entities", "grass", glm::vec3(glm::mat4(m_models["grass"]->poses.front())[3]));

    m_menu.show();
}
