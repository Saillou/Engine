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
    m_center = std::make_unique<Sphere>(0.015f);
    m_center->addRecipe(Cookable::CookType::Solid);
    m_model = std::make_unique<ObjectModel>("Resources/objects/tree/tree.glb");

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
    //UShader& shader = m_model->get(Cookable::CookType::Solid);
    //shader->use();
    //shader->set("diffuse_color", glm::vec4(m_menu.state.color.x, m_menu.state.color.y, m_menu.state.color.z, m_menu.state.color.w));
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
        m_model = std::make_unique<ObjectModel>(m_menu.state.path);
    }

    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_menu.state.centerPosition);
    model = glm::rotate(model, m_menu.state.roll, glm::vec3(1, 0, 0));
    model = glm::rotate(model, m_menu.state.pitch, glm::vec3(0, 1, 0));
    model = glm::rotate(model, m_menu.state.yaw, glm::vec3(0, 0, 1));
    model = glm::translate(model, m_menu.state.centerPosition * -1.f);
    model = glm::translate(model, m_menu.state.position);
    model = glm::scale(model, m_menu.state.scale);

    glm::mat4 worldPos = glm::translate(glm::mat4(1.f), m_menu.state.worldPosition);
    
    m_model->draw(m_window->scene()->camera(), worldPos * model , m_lights);

    UShader& shader = m_center->get(Cookable::CookType::Solid);
    shader->use();
    shader->set("diffuse_color", glm::vec4(1, 1, 1, 1));
    m_center->draw(m_window->scene()->camera(), m_menu.state.centerPosition);

    m_menu.ShowMovableOptions();
}