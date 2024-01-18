#include "Entity.hpp"

#include "Primitive/Cube.hpp"
#include "Primitive/Sphere.hpp"

Entity::Entity(const std::string& path) :
    m_model(path)
{
    addRecipe(CookType::Batch);
    addRecipe(CookType::Model);
    addRecipe(CookType::ModelGeometry);
}

Entity::Entity(SimpleShape shape)
{
    addRecipe(CookType::Batch);
    addRecipe(CookType::Model);
    addRecipe(CookType::ModelGeometry);

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
    auto& sh = *get(CookType::Model);

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

void Entity::drawBatch(const Camera& camera) {
    get(CookType::Batch)->
        use().
        set("View",         camera.modelview).
        set("Projection",   camera.projection);

    m_model.drawElements(*get(CookType::Batch));
}

void Entity::drawElements(Shader& shader) {
    m_model.drawElements(shader);
}

void Entity::setBatch(const std::vector<glm::vec4>& colors, const std::vector<glm::mat4>& models) {
    // TODO: change this
    m_model._root->meshes.front()->updateBatch(colors, models);
}

const Model& Entity::model() const {
    return m_model;
}
