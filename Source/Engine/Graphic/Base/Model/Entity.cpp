#include "Entity.hpp"

#include "Primitive/Cube.hpp"
#include "Primitive/Sphere.hpp"

Entity::Entity(const std::string& path) :
    model(path)
{
    addRecipe(CookType::Basic);
    addRecipe(CookType::Shadow);
    addRecipe(CookType::Geometry);
}

Entity::Entity(SimpleShape shape)
{
    addRecipe(CookType::Basic);
    addRecipe(CookType::Shadow);
    addRecipe(CookType::Geometry);

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

void Entity::drawOne(const Camera& camera, const glm::mat4& quat, const std::vector<Light>& lights) {
    auto& sh = *get(CookType::Basic);

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

void Entity::drawBatch(const Camera& camera, const std::vector<Light>& lights) {
    auto& sh = *get(CookType::Basic);

    sh.use()
        .set("Projection",  camera.projection)
        .set("View",        camera.modelview)
        .set("CameraPos",   camera.position)
        .set("LightNumber", (int)lights.size());

    for (int iLight = 0; iLight < (int)lights.size(); iLight++) {
        sh.set("LightPos_"   + std::to_string(iLight), lights[iLight].position)
          .set("LightColor_" + std::to_string(iLight), lights[iLight].color);
    }

    model.draw(sh);
}

void Entity::drawShadow(const Camera& camera, const Light& light) {
    get(Cookable::CookType::Shadow)->
        use().
        set("Projection",   camera.projection).
        set("View",         camera.modelview).
        set("viewPos",      camera.position).
        set("far_plane",    camera.far_plane).
        set("lightPos",     light.position).
        set("lightColor",   light.color * 0.3f);

    model.draw(*get(Cookable::CookType::Shadow));
}
