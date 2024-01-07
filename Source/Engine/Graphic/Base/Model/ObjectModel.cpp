#include "ObjectModel.hpp"

ObjectModel::ObjectModel(const std::string& path) :
    _object(path)
{
    Cookable::addRecipe(Cookable::CookType::Model);
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

    _object.draw(sh);
}

void ObjectModel::drawElements() {
    _object.drawElements();
}
