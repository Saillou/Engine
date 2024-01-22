#include "MaterialEditor.h"

MaterialEditor::MaterialEditor(Window* window)
    : m_window(window)
{
}

MaterialEditor::~MaterialEditor()
{
}

void MaterialEditor::onEnter()
{
    m_entity = std::make_unique<Entity>(Entity::SimpleShape::Sphere);
    m_entity->poses() = { glm::scale(glm::mat4(1.0f), glm::vec3(0.15f)) };

    m_window->scene().lights() = {
        { glm::vec3{ 0, -0.5f, 0.5f }, glm::vec4{ 1,1,1,1 } }
    };

    m_menu.resetState();
}

void MaterialEditor::onExit()
{
    m_entity.reset();
    m_window->scene().lights().clear();
}

void MaterialEditor::onUpdate()
{
    m_entity->localMaterial().diffuse_color = glm::vec4(m_menu.state.color.x, m_menu.state.color.y, m_menu.state.color.z, m_menu.state.color.w);
    m_window->scene().renderer().draw(m_menu.state.lightEnabled ? Render::DrawType::Lights : Render::DrawType::Basic, *m_entity);

    m_menu.ShowMovableOptions();
}