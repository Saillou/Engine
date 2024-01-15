#include "Entity.hpp"

#include "../Shapes/Cube.hpp"
#include "../Shapes/Sphere.hpp"

Entity::Entity(const std::string& path) :
    m_model(path)
{
    Cookable::addRecipe(Cookable::CookType::Model);
    Cookable::addRecipe(Cookable::CookType::ModelGeometry);
}

Entity::Entity(SimpleShape shape)
{
    Cookable::addRecipe(Cookable::CookType::Model);
    Cookable::addRecipe(Cookable::CookType::ModelGeometry);

    m_model._root = std::make_unique<Model::Node>();

    switch (shape) {
    case Entity::Cube: 
        m_model._root->meshes.push_back(Cube::CreateMesh());
        break;

    case Entity::Sphere: 
        m_model._root->meshes.push_back(Sphere::CreateMesh());
        break;
    }
}

void Entity::draw(const Camera& camera, const glm::mat4& model, const std::vector<Light>& lights) {
    auto& sh = *Cookable::get(Cookable::CookType::Model);

    sh.use()
        .set("Projection",  camera.projection)
        .set("View",        camera.modelview)
        .set("Model",       model)
        .set("CameraPos",   camera.position)
        .set("LightNumber", (int)lights.size());

    for (int iLight = 0; iLight < (int)lights.size(); iLight++) {
        sh.set("LightPos_"   + std::to_string(iLight), lights[iLight].position)
          .set("LightColor_" + std::to_string(iLight), lights[iLight].color);
    }

    m_model.draw(sh);
}

void Entity::drawGeometry(const Camera& camera, const glm::mat4& model) {
    auto& sh = *Cookable::get(Cookable::CookType::ModelGeometry);
    sh.use()
        .set("Projection", camera.projection)
        .set("View", camera.modelview)
        .set("Model", model);

    m_model.draw(sh);
}

void Entity::drawElements(Shader& shader) {
    m_model.drawElements(shader);
}

const Model& Entity::model() const {
    return m_model;
}
