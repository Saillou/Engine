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

    m_lights.push_back({ glm::vec3{ 0, -0.5f, 0.5f }, glm::vec4{ 1,1,1,1 } });

    m_menu.resetState();
}

void MaterialEditor::onExit()
{
    m_entity.reset();
    m_lights.clear();
}

void MaterialEditor::onUpdate()
{
    UShader& shader = m_entity->get(Cookable::CookType::Model);
    shader->use();
    shader->set("diffuse_color", glm::vec4(m_menu.state.color.x, m_menu.state.color.y, m_menu.state.color.z, m_menu.state.color.w));

    if (m_menu.state.lightEnabled)
        m_entity->draw(m_window->scene()->camera(), glm::scale(glm::mat4(1.0f), glm::vec3(0.15f)), m_lights);
    else
        m_entity->draw(m_window->scene()->camera(), glm::scale(glm::mat4(1.0f), glm::vec3(0.15f)));

    m_menu.ShowMovableOptions();
}