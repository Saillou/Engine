#include "Renderer.hpp"
#include <iostream>

Renderer::Renderer()
{

}

void Renderer::draw(Render::DrawType type, const Entity& entity) {
    if (entity.poses().empty()) {
        std::cerr << "Warning: no poses specified, no draws" << std::endl;
        return;
    }

    switch (type)
    {
    case Render::Basic:
        entity.model.draw(
            _setShader(Cookable::CookType::Basic, _camera, {}, nullptr)
            .set("diffuse_color", entity.material.diffuse_color)
        );
        break;

    case Render::Lights:
        entity.model.draw(
            _setShader(Cookable::CookType::Basic, _camera, _lights, nullptr)
            .set("diffuse_color", entity.material.diffuse_color)
        );
        break;

    case Render::Shadows:
        entity.model.draw(
            _setShader(Cookable::CookType::Basic, _camera, _lights, &_shadower)
            .set("diffuse_color", entity.material.diffuse_color)
        );
        break;

    case Render::Geometry:
        entity.model.draw(
            _setShader(Cookable::CookType::Geometry, _camera, {}, nullptr)
            .set("diffuse_color", entity.material.diffuse_color)
        );
        break;
    }
}

void Renderer::text(const std::string& text, float x, float y, float scale, const glm::vec4& color) {
    TextEngine::Write(text, x, y, scale, color);
}

Shader& Renderer::_setShader(Cookable::CookType type, const Camera& camera, const std::vector<Light>& lights, const ShadowRender* shadower) {
    addRecipe(type);

    // Check light capabilities
    const ShaderSource& ssource = get(type)->source(ShaderSource::Type::Fragment);

    // Need to edit shader
    if (ssource.getVar("LightPos").count < lights.size()) {
        editRecipe(type, ShaderSource::Type::Fragment, ShaderSource{}            
            .add_var("uniform", "vec3",        "LightPos",   (int)lights.size())
            .add_var("uniform", "vec4",        "LightColor", (int)lights.size())
            .add_var("uniform", "samplerCube", "depthMap",   (int)lights.size())
        );
    }

    // Use
    Shader& sh = get(type)->use();

    switch (type)
    {
    case Cookable::CookType::Basic:
        if (shadower)
            shadower->bindTextures(GL_TEXTURE0 + 1);

        sh.set("Projection",  camera.projection)
          .set("View",        camera.modelview)
          .set("CameraPos",   camera.position)
          .set("far_plane",   camera.far_plane)
          .set("use_shadow",  shadower != nullptr)
          .set("LightNumber", (int)lights.size());

        for (int iLight = 0; iLight < (int)lights.size(); iLight++) {
            sh.set("LightPos["    + std::to_string(iLight) + "]", lights[iLight].position)
              .set("LightColor["  + std::to_string(iLight) + "]", lights[iLight].color)
              .set("depthMap["    + std::to_string(iLight) + "]", iLight + 1);
        }
        break;

    case Cookable::CookType::Geometry:
        sh.set("Projection", camera.projection)
          .set("View",       camera.modelview);
        break;
    }

    return sh;
}
