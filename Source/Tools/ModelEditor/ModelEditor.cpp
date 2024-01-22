#include "ModelEditor.h"

ModelEditor::ModelEditor(Scene& scene)
    : scene(scene)
{
}

ModelEditor::~ModelEditor()
{
}

void ModelEditor::onEnter()
{
    m_center = std::make_unique<Entity>(Entity::Sphere);
    m_center->localMaterial().diffuse_color = glm::vec4(1, 1, 1, 1);

    m_model = std::make_unique<Entity>("Resources/objects/tree/tree.glb");

    scene.lights() = {
        { glm::vec3{ 0, -0.5f, 0.5f }, glm::vec4{ 1,1,1,1 } }
    };

    m_menu.resetState();
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

    Camera& camera = scene.camera();
    switch (m_menu.state.cameraType)
    {
    case 0:
        scene.camera().position = glm::vec3{ 0, -1, 0 };
        scene.camera().lookAt(glm::vec3(0, 0, 1));
        break;
    case 1:
        scene.camera().position = glm::vec3{ 0,0,1 };
        scene.camera().lookAt(glm::vec3(0, 1, 0));
        break;
    case 2:
        scene.camera().position = glm::vec3{ -1, 0, 0 };
        scene.camera().lookAt(glm::vec3(0, 0, 1));
        break;
    }

    if (m_menu.state.updateAsset)
    {
        m_menu.state.updateAsset = false;
        m_model = std::make_unique<Entity>(m_menu.state.path);
    }

    auto& renderer = scene.renderer();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_menu.state.centerPosition);
    model = glm::rotate(model, m_menu.state.roll, glm::vec3(1, 0, 0));
    model = glm::rotate(model, m_menu.state.pitch, glm::vec3(0, 1, 0));
    model = glm::rotate(model, m_menu.state.yaw, glm::vec3(0, 0, 1));
    model = glm::translate(model, m_menu.state.centerPosition * -1.f);
    model = glm::translate(model, m_menu.state.position);
    model = glm::scale(model, m_menu.state.scale);

    glm::mat4 worldPos = glm::translate(glm::mat4(1.f), m_menu.state.worldPosition);
    m_model->poses() = { worldPos * model };
    renderer.draw(Render::DrawType::Lights, *m_model);

    m_center->poses() = { glm::scale(glm::translate(glm::mat4(1.0f), m_menu.state.centerPosition), glm::vec3(0.015f)) };
    renderer.draw(Render::DrawType::Basic, *m_center);
    m_menu.ShowMovableOptions();
}