#include "Entity.hpp"

#include "Primitive/Cube.hpp"
#include "Primitive/Sphere.hpp"

Entity::Entity(const std::string& path) :
    model(path)
{
    addRecipe(CookType::Batch);
    addRecipe(CookType::BatchGeometry);
}

Entity::Entity(SimpleShape shape)
{
    addRecipe(CookType::Batch);
    addRecipe(CookType::BatchGeometry);

    model._root = std::make_unique<Model::Node>();

    switch (shape) {
    case Entity::Cube: 
        model._root->meshes.push_back(Cube::CreateMesh());
        break;

    case Entity::Sphere: 
        model._root->meshes.push_back(Sphere::CreateMesh());
        break;
    }
}

void Entity::draw(const Camera& camera, const glm::mat4& quat, const std::vector<Light>& lights) {
    auto& sh = *get(CookType::Batch);

    sh.use()
        .set("Projection",  camera.projection)
        .set("View",        camera.modelview)
        .set("CameraPos",   camera.position)
        .set("LightNumber", (int)lights.size());

    for (int iLight = 0; iLight < (int)lights.size(); iLight++) {
        sh.set("LightPos_"   + std::to_string(iLight), lights[iLight].position)
          .set("LightColor_" + std::to_string(iLight), lights[iLight].color);
    }

    model.setBatch({}, { quat });
    model.draw(sh);
}

void Entity::drawBatch(const Camera& camera) {
    get(CookType::Batch)->
        use().
        set("View",         camera.modelview).
        set("Projection",   camera.projection);

    model.drawElements(*get(CookType::Batch));
}