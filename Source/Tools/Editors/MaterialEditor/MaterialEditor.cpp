#include "MaterialEditor.hpp"

MaterialEditor::MaterialEditor(Scene& scene) :
    Editor(scene)
{ }

void MaterialEditor::onEnter()
{
    m_menu.reset();

    // Scene lights
    m_scene.lights = {
        { glm::vec3{ 0, -0.5f, 0.5f }, glm::vec4{ 1,1,1,1 } }
    };
    m_menu.state.lightEnabled = true;

    // Camera
    m_scene.camera.position = glm::vec3(0.0f, -1.0f, 0.15f);
    m_scene.camera.direction = glm::vec3(0, 0, 0);

    // Entity
    m_sphere = ManagedEntity::Create(Model::Load(Model::SimpleShape::Sphere));
    m_sphere->local() = {
        glm::scale(glm::mat4(1.0f), glm::vec3(0.15f))
    };
}

void MaterialEditor::onExit()
{
    m_sphere.reset();
}

void MaterialEditor::onUpdate()
{
    m_sphere->color() = glm::vec4(m_menu.state.color.x, m_menu.state.color.y, m_menu.state.color.z, m_menu.state.color.w);

    // Enable lights
    if (m_scene.lights.empty() && m_menu.state.lightEnabled) {
        m_scene.lights = {
            { glm::vec3{ 0, -0.5f, 0.5f }, glm::vec4{ 1,1,1,1 } }
        };
    }
    
    // Disable lights
    if (!m_scene.lights.empty() && !m_menu.state.lightEnabled) {
        m_scene.lights.clear();
    }

    m_menu.show();
}