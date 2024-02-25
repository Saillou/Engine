#include "ModelEditor.hpp"

ModelEditor::ModelEditor(Scene& scene)
    : Editor(scene)
{ }


void ModelEditor::onEnter()
{
    m_menu.resetState();

    // Lights
    m_scene.lights = {
        { glm::vec3{ 0, -0.5f, 0.5f }, glm::vec4{ 1,1,1,1 } }
    };

    // Entities
    m_center = ManagedEntity::Create(Model::Load(Model::SimpleShape::Sphere));
    m_center->color() = glm::vec4(1, 1, 1, 1);

    m_model = ManagedEntity::Create(Model::Load("Resources/objects/tree/tree.glb"));
}

void ModelEditor::onExit()
{
    m_center.reset();
    m_model.reset();

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

    Camera& camera = m_scene.camera;
    switch (m_menu.state.cameraType)
    {
    case 0:
        m_scene.camera.position = glm::vec3{ 0, -1, 0 };
        m_scene.camera.up = glm::vec3(0, 0, 1);
        m_scene.camera.lookAt();
        break;

    case 1:
        m_scene.camera.position = glm::vec3{ 0,0,1 };
        m_scene.camera.up = glm::vec3(0, 1, 0);
        m_scene.camera.lookAt();
        break;

    case 2:
        m_scene.camera.position = glm::vec3{ -1, 0, 0 };
        m_scene.camera.up = glm::vec3(0, 0, 1);
        m_scene.camera.lookAt();
        break;
    }

    if (m_menu.state.updateAsset)
    {
        m_menu.state.updateAsset = false;
        m_model = ManagedEntity::Create(Model::Load(m_menu.state.path));
    }

    m_menu.ShowMovableOptions();

    // Model
    glm::mat4& model = m_model->local();
    model = glm::translate(glm::mat4(1.0f), m_menu.state.centerPosition);
    model = glm::rotate(model, m_menu.state.roll, glm::vec3(1, 0, 0));
    model = glm::rotate(model, m_menu.state.pitch, glm::vec3(0, 1, 0));
    model = glm::rotate(model, m_menu.state.yaw, glm::vec3(0, 0, 1));
    model = glm::translate(model, m_menu.state.centerPosition * -1.f);
    model = glm::translate(model, m_menu.state.position);
    model = glm::scale(model, m_menu.state.scale);

    m_model->world() = glm::translate(glm::mat4(1.f), m_menu.state.worldPosition);

    // Center
    m_center->local() = {glm::scale(glm::translate(glm::mat4(1.0f), m_menu.state.centerPosition), glm::vec3(0.015f))};
}