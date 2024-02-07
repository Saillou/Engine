#include "MaterialEditor.h"

MaterialEditor::MaterialEditor(Scene& scene) :
    Editor(scene)
{ }

void MaterialEditor::onEnter()
{
    m_entity = std::make_unique<Entity>(Entity::SimpleShape::Sphere);
    m_entity->poses() = { glm::scale(glm::mat4(1.0f), glm::vec3(0.15f)) };

    m_scene.lights() = {
        { glm::vec3{ 0, -0.5f, 0.5f }, glm::vec4{ 1,1,1,1 } }
    };

    m_scene.camera().position = glm::vec3(0.0f, -1.0f, 0.15f);
    m_scene.camera().direction = glm::vec3(0, 0, 0);

    m_menu.reset();
}

void MaterialEditor::onExit()
{
    m_entity.reset();
}

void MaterialEditor::onUpdate()
{
    m_entity->localMaterial().diffuse_color = glm::vec4(m_menu.state.color.x, m_menu.state.color.y, m_menu.state.color.z, m_menu.state.color.w);
    m_scene.renderer().draw(m_menu.state.lightEnabled ? Render::DrawType::Lights : Render::DrawType::Basic, *m_entity);

    m_menu.show();
}