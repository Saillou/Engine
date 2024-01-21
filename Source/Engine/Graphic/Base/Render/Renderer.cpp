#include "Renderer.hpp"

#include <iostream>

void Renderer::draw(Render::DrawType type, Entity& entity) {
    if (_deferred) {
        _DrawEntity di;
        di.type = type;
        di._model = entity._model;
        di._localPose = entity._localPose;
        di._localMaterial = entity._localMaterial;
        di._poses = entity._poses;
        di._materials = entity._materials;

        _heapEntities.emplace_back(std::move(di));
    }
    else {
        Shader placeHolder;
        entity._update_model_buffer();
        entity._model->draw([&]() -> Shader& {
            switch (type) {
                case Render::Basic:    return _setShader(Cookable::CookType::Basic,    _camera, {},      nullptr);
                case Render::Lights:   return _setShader(Cookable::CookType::Basic,    _camera, _lights, nullptr);
                case Render::Shadows:  return _setShader(Cookable::CookType::Basic,    _camera, _lights, &_shadower);
                case Render::Geometry: return _setShader(Cookable::CookType::Geometry, _camera, {},      nullptr);
            } return placeHolder;
        }().set("diffuse_color", entity._localMaterial.diffuse_color));
    }
}

void Renderer::text(const std::string& text, float x, float y, float scale, const glm::vec4& color) {
    if (_deferred) {
        _DrawText dt;
        dt.text = text;
        dt.x = x;
        dt.y = y;
        dt.scale = scale;
        dt.color = color;

        _heapText.emplace_back(std::move(dt));
    }
    else {
        TextEngine::Write(text, x, y, scale, color);
    }
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

    switch (type) {
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

void Renderer::_clear() {
    _heapEntities.clear();
    _heapText.clear();
}

void Renderer::_compute() 
{
    _shadower.render(_camera, _lights, [=](Shader& sh) {
        for (_DrawEntity& di : _heapEntities) {
            di._model->_localPose = glm::mat4(di._localPose);
            di._model->_setBatch(std::vector<glm::mat4>(di._poses.cbegin(), di._poses.cend()), Material::ExtractColors(di._materials));

            if (!di._localMaterial.cast_shadow)
                continue;

            di._model->drawElements(sh);
        }
    });
}

void Renderer::_draw() {
    for (_DrawEntity& di : _heapEntities) {
        di._model->_localPose = glm::mat4(di._localPose);
        di._model->_setBatch(std::vector<glm::mat4>(di._poses.cbegin(), di._poses.cend()), Material::ExtractColors(di._materials));

        Shader placeHolder;
        di._model->draw([&]() -> Shader& {
            switch (di.type) {
                case Render::Basic:    return _setShader(Cookable::CookType::Basic,    _camera, {},      nullptr);
                case Render::Lights:   return _setShader(Cookable::CookType::Basic,    _camera, _lights, nullptr);
                case Render::Shadows:  return _setShader(Cookable::CookType::Basic,    _camera, _lights, &_shadower);
                case Render::Geometry: return _setShader(Cookable::CookType::Geometry, _camera, {},      nullptr);
            } return placeHolder;
        }().set("diffuse_color", di._localMaterial.diffuse_color));
    }

    for (_DrawText& dt : _heapText) {
        TextEngine::Write(dt.text, dt.x, dt.y, dt.scale, dt.color);
    }
}
