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
    m_model = std::make_unique<ObjectModel>("Resources/objects/tree/tree.glb");

    m_lights.push_back({ glm::vec3{ 0, -0.5f, 0.5f }, glm::vec4{ 1,1,1,1 } });

    m_menu.resetState();
}

void ModelEditor::onExit()
{
    m_model.reset();
    m_lights.clear();
}

void ModelEditor::onUpdate()
{
    //UShader& shader = m_model->get(Cookable::CookType::Solid);
    //shader->use();
    //shader->set("diffuse_color", glm::vec4(m_menu.state.color.x, m_menu.state.color.y, m_menu.state.color.z, m_menu.state.color.w));
    if (m_menu.state.updateAsset)
    {
        m_menu.state.updateAsset = false;
        m_model = std::make_unique<ObjectModel>(m_menu.state.path);
    }


    glm::mat4 quat = glm::scale(
        glm::rotate(
            glm::translate(glm::mat4(1.0f),    
                m_menu.state.position),
                m_menu.state.rotation, m_menu.state.rotationVector),
        m_menu.state.scale                                              
    );
    m_model->draw(m_window->scene()->camera(), quat, m_lights);

    m_menu.ShowMovableOptions();
}