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
    m_model = std::make_unique<Sphere>(0.15f, 6);
    m_model->addRecipe(Cookable::CookType::Solid);

    m_lights.push_back({ glm::vec3{ 0, -0.5f, 0.5f }, glm::vec4{ 1,1,1,1 } });

    m_menu.resetState();
}

void MaterialEditor::onExit()
{
    m_model.reset();
    m_lights.clear();
}

void MaterialEditor::onUpdate()
{
    UShader& shader = m_model->get(Cookable::CookType::Solid);
    shader->use();
    shader->set("diffuse_color", glm::vec4(m_menu.state.color.x, m_menu.state.color.y, m_menu.state.color.z, m_menu.state.color.w));

    if (m_menu.state.lightEnabled)
        m_model->draw(m_window->scene()->camera(), {}, {}, m_lights);
    else
        m_model->draw(m_window->scene()->camera());

    m_menu.ShowMovableOptions();
}