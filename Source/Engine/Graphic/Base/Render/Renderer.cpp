#include "Renderer.hpp"
#include <iostream>
Renderer::Renderer()
{
	addRecipe(CookType::Basic);
	addRecipe(CookType::Geometry);
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

Shader& Renderer::_setShader(Cookable::CookType type, const Camera& camera, const std::vector<Light>& lights, const ShadowRender* shadower) {
    Shader& sh = get(type)->use();

    switch (type)
    {
    case Cookable::CookType::Basic:
        if (shadower)
            shadower->bindTextures(GL_TEXTURE0 + 1);

        sh.set("Projection",    camera.projection)
            .set("View",        camera.modelview)
            .set("CameraPos",   camera.position)
            .set("far_plane",   camera.far_plane)
            .set("use_shadow",  shadower != nullptr)
            .set("LightNumber", (int)lights.size());

        for (int iLight = 0; iLight < (int)lights.size(); iLight++) {
            sh.set("LightPos["      + std::to_string(iLight) + "]", lights[iLight].position)
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
