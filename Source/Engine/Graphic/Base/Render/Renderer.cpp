#include "Renderer.hpp"

#include "../../../Utils/RayCaster.hpp"
#include <iostream>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>

void Renderer::draw(Render::DrawType type, Entity& entity) {
    if (!deferred)
        return _drawEntitySync(type, entity);

    _DrawEntity di;
    di.drawId         = _heapEntities.size() + 1;
    di.drawPriority   = -1.0f;
    di.type           = type;
    di.copied_entity  = entity;

    _heapEntities.emplace_back(std::move(di));
}

void Renderer::text(const std::string& text, float x, float y, float scale, const glm::vec4& color) {
    if(!deferred)
        return TextEngine::Write(text, x, y, scale, color);

    _DrawText dt;
    dt.text = text;
    dt.x = x;
    dt.y = y;
    dt.scale = scale;
    dt.color = color;

    _heapText.emplace_back(std::move(dt));
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
    _entitiesDuplicates.clear();
}

void Renderer::_compute() 
{
    // Apply batch transformations
    std::unordered_map<std::shared_ptr<Model>, size_t> first_model_id;
    for (_DrawEntity& di : _heapEntities) {
        // Decide draw order:
        //  - opaque first: no order
        //  - transparent: furthest first
        di.drawPriority = std::numeric_limits<float>::max();

        if (di.copied_entity.localMaterial().diffuse_color.a < 1.0f) {
            // Sort also poses
            using dist_entity = std::pair<float, Pose>;
            std::vector<dist_entity> all_dist_entity;

            for (const Pose& pose : di.copied_entity.poses()) {
                float entity_distance = RayCaster::OrientedDistance(_camera.position, di.copied_entity, pose);

                all_dist_entity.push_back({ entity_distance, pose });
                di.drawPriority = std::min(entity_distance, di.drawPriority);
            }

            std::sort(all_dist_entity.begin(), all_dist_entity.end(), [=](const dist_entity& di1, const dist_entity& di2) {
                return di1.first > di2.first;
            });

            std::vector<Pose> sorted_poses;
            for (const dist_entity& de: all_dist_entity) {
                sorted_poses.push_back(de.second);
            }

            di.copied_entity.poses() = sorted_poses;
        }

        // Check if models already used for another draw
        if (first_model_id.find(di.copied_entity._model) != first_model_id.cend()) {
            _entitiesDuplicates.insert(di.drawId);

            // First one will also need to be redrawn
            if (_entitiesDuplicates.find(first_model_id[di.copied_entity._model]) == _entitiesDuplicates.cend())
                _entitiesDuplicates.insert(first_model_id[di.copied_entity._model]);
            continue; // gpu buffer needs to be setup after
        }

        // Update model buffers
        first_model_id.emplace(di.copied_entity._model, di.drawId);
        di.copied_entity._update_model_buffer();
    }

    // Re-order vector from last to first
    std::sort(_heapEntities.begin(), _heapEntities.end(), [=](const _DrawEntity& di1, const _DrawEntity& di2) {
        return di1.drawPriority > di2.drawPriority;
    });
    
    // Shadows
    _shadower.render(_camera, _lights, [=](Shader& sh) {
        for (_DrawEntity& di : _heapEntities) {
            if (!di.copied_entity._localMaterial.cast_shadow)
                continue;

            if (di.drawPriority < 0.0f)
                break;

            // Need to re-set batch models
            if (_entitiesDuplicates.find(di.drawId) != _entitiesDuplicates.cend()) {
                di.copied_entity._update_model_buffer();
            }

            di.copied_entity._model->drawElements(sh);
        }
    });
}

void Renderer::_draw() {
    for (_DrawEntity& di : _heapEntities) {
        if (di.drawPriority < 0.0f)
            break;

        // Need to re-set batch models
        if (_entitiesDuplicates.find(di.drawId) != _entitiesDuplicates.cend()) {
            di.copied_entity._update_model_buffer();
        }

        _drawEntitySync(di.type, di.copied_entity, false);
    }

    for (_DrawText& dt : _heapText) {
        TextEngine::Write(dt.text, dt.x, dt.y, dt.scale, dt.color);
    }
}

void Renderer::_drawEntitySync(Render::DrawType type, Entity& entity, bool update_buffer) {
    Shader placeHolder;

    if(update_buffer)
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
