#include "Renderer.hpp"

#include "../../../Utils/Timer.hpp"
#include "../../../Utils/RayCaster.hpp"
#include <iostream>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>

// Rendering
void Renderer::quad(const Quad& surface) {
    if (!_deferred)
        return _drawQuadSync(surface);

    std::cerr << "[Warning] Renderer: can't draw quad in deferred mode. (Wrap these in an entity instead)" << std::endl;
}

void Renderer::draw(Render::DrawType type, Entity& entity) {
    if (!_deferred)
        return _drawEntitySync(type, entity);

    _DrawEntity di;
    di.drawId         = _heapEntities.size() + 1;
    di.drawPriority   = -1.0f;
    di.type           = type;
    di.copied_entity  = entity;

    _heapEntities.emplace_back(std::move(di));
}

void Renderer::draw(const std::string& shaderName, Entity& entity) {
    if (_userShaders.find(shaderName) == _userShaders.cend())
    {
        std::cerr << "Shader not found. Abort draw." << std::endl;
        return;
    }

    draw(_userShaders[shaderName].type, entity);
}

void Renderer::text(const std::string& text, float x, float y, float scale, const glm::vec4& color) {
    if(!_deferred)
        return _drawTextSync(text, x, y, scale, color);

    _DrawText dt;
    dt.text = text;
    dt.x = x;
    dt.y = y;
    dt.scale = scale;
    dt.color = color;

    _heapText.emplace_back(std::move(dt));
}

// Public
void Renderer::add_shader(const std::string& shaderName, const ShaderGetter& getter, const ShaderSetter& setter) {
    _userShaders[shaderName] = {
        Render::DrawType(Render::Custom + _userShaders.size()),
        getter,
        setter
    };
    _userShadersName[_userShaders[shaderName].type] = shaderName;
}

void Renderer::remove_shader(const std::string& shaderName) {
    if (_userShaders.find(shaderName) == _userShaders.cend())
        return;

    _userShadersName.erase(_userShaders[shaderName].type);
    _userShaders.erase(shaderName);
}

// Private
Shader& Renderer::_setShader(Cookable::CookType type, const Camera& camera, const std::vector<Light>& lights, const ShadowRender* shadower) {
    addRecipe(type);

    // Use
    Shader& sh = get(type)->use();

    switch (type) {
    case Cookable::CookType::Basic:
        {
            // Check light capabilities
            const ShaderSource& ssource = get(type)->source(ShaderSource::Type::Fragment);

            // Need to edit shader
            if (ssource.getVar("LightPos").count < lights.size()) {
                editRecipe(type, ShaderSource::Type::Fragment, ShaderSource{}
                    .add_var("uniform", "vec3", "LightPos", (int)lights.size())
                    .add_var("uniform", "vec4", "LightColor", (int)lights.size())
                    .add_var("uniform", "samplerCube", "depthMap", (int)lights.size())
                );
            }

            // Bind shadow map
            if (shadower)
                shadower->bindTextures(GL_TEXTURE0 + 1);

            // Set uniforms
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
        }
        break;

    case Cookable::CookType::Particle:
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

        if (di.copied_entity.localMaterial().diffuse_color.a < 1.0f || di.copied_entity.localMaterial().force_reorder) {
            // Sort also poses in batch
            using dist_entity = std::tuple<float, Pose, Material>;
            std::vector<dist_entity> all_dist_entity;
            all_dist_entity.reserve(di.copied_entity.materials().size());

            size_t i_element = 0;
            auto& materials = di.copied_entity.materials();

            for (const Pose& pose : di.copied_entity.poses()) {
                float entity_distance = RayCaster::ApproxDistance(_camera.position, di.copied_entity, pose);

                all_dist_entity.push_back({
                    entity_distance, 
                    pose, 
                    i_element < materials.size() ? materials[i_element] : Material{{0,0,0,0}}
                });

                di.drawPriority = std::min(entity_distance, di.drawPriority);

                i_element++;
            }

            std::sort(all_dist_entity.begin(), all_dist_entity.end(), [=](const dist_entity& di1, const dist_entity& di2) {
                return std::get<0>(di1) > std::get<0>(di2);
            });

            std::vector<Pose> sorted_poses;          sorted_poses.reserve(all_dist_entity.size());
            std::vector<Material> sorted_materials;  sorted_materials.reserve(all_dist_entity.size());

            for (const dist_entity& de: all_dist_entity) {
                sorted_poses.push_back(std::get<1>(de));
                sorted_materials.push_back(std::get<2>(de));
            }

            di.copied_entity.setPosesWithMaterials(sorted_poses, sorted_materials);
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
        _drawTextSync(dt.text, dt.x, dt.y, dt.scale, dt.color);
    }
}

void Renderer::_drawQuadSync(const Quad& surface) {
    addRecipe(Cookable::CookType::Shape);

    Shader& sh = get(Cookable::CookType::Shape)->use()
        .set("LocalModel",       surface.absolute_dimmensions ? 
            glm::translate(glm::scale(glm::mat4(1.0f), 
                glm::vec3(surface.w()/2.0f, surface.h()/2.0f, 0.0f)),
                glm::vec3(
                    +1.0f + surface.x()/(surface.w() / 2.0f), 
                    -1.0f + (_camera.screenSize.y - surface.y())/(surface.h() / 2.0f),
                0)) : surface.pose())
        .set("projection",       surface.absolute_dimmensions ?
            glm::ortho(0.0f, _camera.screenSize.x, 0.0f, _camera.screenSize.y):
            glm::mat4(1.0f))
        .set("quadTexture",      surface.texture_location)
        .set("background_color", surface.material.diffuse_color);

    surface.drawElements();
}

void Renderer::_drawEntitySync(Render::DrawType type, Entity& entity, bool update_buffer) {
    Shader placeHolder;

    if(update_buffer)
        entity._update_model_buffer();

    if(type < Render::Custom) {
        entity._model->draw([&]() -> Shader& {
            switch (type) {
                case Render::Basic:    return _setShader(Cookable::CookType::Basic,    _camera, {},      nullptr);
                case Render::Lights:   return _setShader(Cookable::CookType::Basic,    _camera, _lights, nullptr);
                case Render::Shadows:  return _setShader(Cookable::CookType::Basic,    _camera, _lights, &_shadower);
                case Render::Geometry: return _setShader(Cookable::CookType::Geometry, _camera, {},      nullptr);
                case Render::Particle: return _setShader(Cookable::CookType::Particle, _camera, {},      nullptr);
            } return placeHolder;
        }().set("diffuse_color", entity._localMaterial.diffuse_color));
    }
    else {
        if (_userShadersName.find(type) == _userShadersName.cend())
            return;

        _UserShaderMemo& _usrShader = _userShaders[_userShadersName[type]];
        _usrShader.setter(entity);
        entity._model->draw(_usrShader.getter());
    }
}

void Renderer::_drawTextSync(const std::string& text, float x, float y, float scale, const glm::vec4& color) {
    TextEngine::Write(text, x, y, scale, color);
}
