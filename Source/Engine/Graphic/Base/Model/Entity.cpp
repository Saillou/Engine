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

// - - - - - - - - - - - - - - - - - - 

void Entity::drawOne(Cookable::CookType type, const Camera& camera, const glm::mat4& quat, const std::vector<Light>& lights) {
    Shader& sh = get(type)->use();

    // I don't like that but ok for now
    switch (type)
    {
    case Cookable::CookType::Basic:
        sh.set("Projection",  camera.projection)
          .set("View",        camera.modelview)
          .set("CameraPos",   camera.position)
          .set("LightNumber", (int)lights.size());

        for (int iLight = 0; iLight < (int)lights.size(); iLight++) {
            sh.set("LightPos_"   + std::to_string(iLight), lights[iLight].position)
              .set("LightColor_" + std::to_string(iLight), lights[iLight].color);
        }
        break;

    case Cookable::CookType::Shadow:
        sh.set("Projection", camera.projection)
          .set("View",       camera.modelview)
          .set("viewPos",    camera.position)
          .set("far_plane",  camera.far_plane)
          .set("lightPos",   lights[0].position)
          .set("lightColor", lights[0].color * 0.3f);
        break;

    case Cookable::CookType::Geometry:
        sh.set("Projection", camera.projection)
          .set("View",       camera.modelview);
        break;
    }

    // Careful it will affects subsequent draw
    model.setBatch({}, { quat });

    model.draw(sh);
}

// - - - - - - - - - - - - - - - - - -  

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

void Entity::drawGeometry(const Camera& camera) {
    get(Cookable::CookType::Geometry)->
        use().
        set("Projection", camera.projection).
        set("View",       camera.modelview);

    model.drawElements(*get(Cookable::CookType::Geometry));
}
