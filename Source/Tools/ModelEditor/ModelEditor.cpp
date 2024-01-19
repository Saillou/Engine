#include "ModelEditor.h"

ModelEditor::ModelEditor(Window* window)
    : m_window(window)
{
}

ModelEditor::~ModelEditor()
{
}

void ModelEditor::onEnter()
{
    m_center = std::make_unique<Entity>(Entity::Sphere);
    m_model = std::make_unique<Entity>("Resources/objects/tree/tree.glb");

    m_lights.push_back({ glm::vec3{ 0, -0.5f, 0.5f }, glm::vec4{ 1,1,1,1 } });

    m_menu.resetState();
}

void ModelEditor::onExit()
{
    m_center.reset();
    m_model.reset();
    m_lights.clear();

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
}

void ModelEditor::onUpdate()
{
    if (m_menu.state.showWireFrame)
    {
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
    }

    Camera& camera = m_window->scene()->camera();
    switch (m_menu.state.cameraType)
    {
    case 0:
        m_window->scene()->camera().position = glm::vec3{ 0, -1, 0 };
        m_window->scene()->camera().lookAt(glm::vec3(0, 0, 1));
        break;
    case 1:
        m_window->scene()->camera().position = glm::vec3{ 0,0,1 };
        m_window->scene()->camera().lookAt(glm::vec3(0, 1, 0));
        break;
    case 2:
        m_window->scene()->camera().position = glm::vec3{ -1, 0, 0 };
        m_window->scene()->camera().lookAt(glm::vec3(0, 0, 1));
        break;
    }

    if (m_menu.state.updateAsset)
    {
        m_menu.state.updateAsset = false;
        m_model = std::make_unique<Entity>(m_menu.state.path);
    }

    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_menu.state.centerPosition);
    model = glm::rotate(model, m_menu.state.roll, glm::vec3(1, 0, 0));
    model = glm::rotate(model, m_menu.state.pitch, glm::vec3(0, 1, 0));
    model = glm::rotate(model, m_menu.state.yaw, glm::vec3(0, 0, 1));
    model = glm::translate(model, m_menu.state.centerPosition * -1.f);
    model = glm::translate(model, m_menu.state.position);
    model = glm::scale(model, m_menu.state.scale);

    glm::mat4 worldPos = glm::translate(glm::mat4(1.f), m_menu.state.worldPosition);
    
    m_model->drawOne(Cookable::CookType::Basic, m_window->scene()->camera(), worldPos * model , m_lights);

    m_center->get(Cookable::CookType::Basic)->use().set("diffuse_color", glm::vec4(1, 1, 1, 1));
    m_center->drawOne(Cookable::CookType::Basic, m_window->scene()->camera(), glm::scale(glm::translate(glm::mat4(1.0f), m_menu.state.centerPosition), glm::vec3(0.015f)));

    m_menu.ShowMovableOptions();
}