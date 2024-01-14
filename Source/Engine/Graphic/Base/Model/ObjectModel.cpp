#include "ObjectModel.hpp"

ObjectModel::ObjectModel(const std::string& path) :
    _model(path)
{
    Cookable::addRecipe(Cookable::CookType::Model);
    Cookable::addRecipe(Cookable::CookType::ModelGeometry);
}

void ObjectModel::draw(const Camera& camera, const glm::mat4& model, const std::vector<Light>& lights) {
    auto& sh = *Cookable::get(Cookable::CookType::Model);
    sh.use()
        .set("Projection",  camera.projection)
        .set("View",        camera.modelview)
        .set("Model",       model)
        .set("CameraPos",   camera.position)
        .set("LightNumber", (int)lights.size());

    for (int iLight = 0; iLight < (int)lights.size(); iLight++) {
        sh
            .set("LightPos_" + std::to_string(iLight), lights[iLight].position)
            .set("LightColor_" + std::to_string(iLight), lights[iLight].color);
    }

    _model.draw(sh);
}

void ObjectModel::drawGeometry(const Camera& camera, const glm::mat4& model) {
    auto& sh = *Cookable::get(Cookable::CookType::ModelGeometry);
    sh.use()
        .set("Projection", camera.projection)
        .set("View", camera.modelview)
        .set("Model", model);

    _model.draw(sh);
}

void ObjectModel::drawElements(Shader& shader) {
    _model.drawElements(shader);
}

const Model& ObjectModel::model() const {
    return _model;
}
