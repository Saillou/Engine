#include "Entity.hpp"

#include "Primitive/Cube.hpp"
#include "Primitive/Sphere.hpp"

Entity::Entity(const std::string& path) :
    model(path)
{
    addRecipe(CookType::Basic);
    addRecipe(CookType::Geometry);
}

Entity::Entity(SimpleShape shape)
{
    addRecipe(CookType::Basic);
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

void Entity::drawOne(Cookable::CookType type, const Camera& camera, const glm::mat4& quat, const std::vector<Light>& lights, const ShadowRender* shadower) {
    // Careful: it will affects subsequent draw
    model.setBatch({ quat });
    _setShader(type, camera, lights, shadower);
    model.draw(*get(type));
}

void Entity::drawBasic(const Camera& camera, const std::vector<Light>& lights, const ShadowRender* shadower) {
    _setShader(Cookable::CookType::Basic, camera, lights, shadower);
    model.draw(*get(Cookable::CookType::Basic));
}

void Entity::drawGeometry(const Camera& camera) {
    _setShader(Cookable::CookType::Geometry, camera);
    model.draw(*get(Cookable::CookType::Geometry));
}

void Entity::_setShader(Cookable::CookType type, const Camera& camera, const std::vector<Light>& lights, const ShadowRender* shadower) {
    Shader& sh = get(type)->use();

    // I don't like that but ok for now
    switch (type)
    {
    case Cookable::CookType::Basic:
        if(shadower)
            shadower->bindTextures(GL_TEXTURE0 + 1);

        sh.set("Projection",  camera.projection)
          .set("View",        camera.modelview)
          .set("CameraPos",   camera.position)
          .set("far_plane",   camera.far_plane)
          .set("use_shadow",  shadower != nullptr)
          .set("LightNumber", (int)lights.size());

        for (int iLight = 0; iLight < (int)lights.size(); iLight++) {
            sh.set("LightPos["   + std::to_string(iLight) + "]", lights[iLight].position)
              .set("LightColor[" + std::to_string(iLight) + "]", lights[iLight].color)
              .set("depthMap["   + std::to_string(iLight) + "]", iLight + 1);
        }
        break;

    case Cookable::CookType::Geometry:
        sh.set("Projection", camera.projection)
          .set("View",       camera.modelview);
        break;
    }
}
