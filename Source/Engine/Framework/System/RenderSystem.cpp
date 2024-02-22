#include "RenderSystem.hpp"

#include "../../Graphic/Base/Scene.hpp"
#include "../../Utils/Timer.hpp"

#include <iostream>
#include <algorithm>

RenderSystem::RenderSystem(Camera& camera, std::vector<Light>& lights) :
    _camera(camera),
    _lights(lights)
{
}

void RenderSystem::init() 
{
    Signature signature;

    signature.set(ECS::getComponentType<BodyComponent>());
    signature.set(ECS::getComponentType<DrawComponent>());

    ECS::setSystemSignature<RenderSystem>(signature);
}

void RenderSystem::update()
{
    Scene::Viewport(_shadower.width(), _shadower.height());
    _compute();
    _drawShadows();

    Scene::Viewport((int)_camera.screenSize.x, (int)_camera.screenSize.y);
    for (auto& entity : m_entities) {
        _drawEntity(entity);
    }
}

// Helpers
Shader& RenderSystem::_setShader(Cookable::CookType type, const Camera& camera, const std::vector<Light>& lights, const ShadowRender* shadower) {
    addRecipe(type);

    switch (type) {
    case Cookable::CookType::Basic:
        {
            // Check light capabilities
            if (get(type)->source(ShaderSource::Type::Fragment).getVar("LightPos").count < lights.size())
            {
                editRecipe(type, ShaderSource::Type::Fragment, ShaderSource{}
                    .add_var("uniform", "vec3", "LightPos", (int)lights.size())
                    .add_var("uniform", "vec4", "LightColor", (int)lights.size())
                    .add_var("uniform", "samplerCube", "depthMap", (int)lights.size())
                );
            }

            // Use
            Shader& sh = get(type)->use();

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

            for (int iLight = 0; iLight < (int)lights.size(); iLight++) 
            {
                sh.set("LightPos["    + std::to_string(iLight) + "]", lights[iLight].position)
                  .set("LightColor["  + std::to_string(iLight) + "]", lights[iLight].color)
                  .set("depthMap["    + std::to_string(iLight) + "]", iLight + 1);
            }
        }
        break;

    case Cookable::CookType::Particle:
    case Cookable::CookType::Geometry:
        Shader& sh = get(type)->use();

        sh.set("Projection", camera.projection)
          .set("View",       camera.modelview);
        break;
    }

    return *get(type);
}

// Prepare draw
void RenderSystem::_compute() 
{
    // Create batches here
    std::unordered_map<Model::Ref, std::vector<glm::mat4>> _batches;

    // Get info
    for (auto& entity : m_entities) {
        const DrawComponent&  draw  = ECS::getComponent<DrawComponent>(entity);
        const BodyComponent&  body  = ECS::getComponent<BodyComponent>(entity);

        if (draw.type == DrawComponent::Hidden)
            continue;

        _batches[body.model].push_back(body.transform);
    }

    // Set batches
    for (auto& it : _batches) {
        it.first->_setBatch(it.second);
    }
}

// Draws
void RenderSystem::_drawShadows() 
{
    _shadower.render(_camera, _lights, [=](Shader& sh) 
        {
            for (auto& entity: m_entities) {
                DrawComponent& draw = ECS::getComponent<DrawComponent>(entity);
                BodyComponent& body = ECS::getComponent<BodyComponent>(entity);

                if (draw.type == DrawComponent::Hidden)
                    continue;

                //if (!bodyComponent.model->material.cast_shadow)
                //    continue;

                body.model->drawElements(sh);
            }
        }
    );
}

void RenderSystem::_drawEntity(Entity entity) 
{
    DrawComponent& draw = ECS::getComponent<DrawComponent>(entity);
    BodyComponent& body = ECS::getComponent<BodyComponent>(entity);

    if (draw.type == DrawComponent::Hidden)
        return;

    if(draw.type < DrawComponent::Custom) {
        body.model->draw([&]() -> Shader& {
            switch (draw.type) {
                case DrawComponent::Basic:    return _setShader(Cookable::CookType::Basic,    _camera, {},      nullptr);
                case DrawComponent::Lights:   return _setShader(Cookable::CookType::Basic,    _camera, _lights, nullptr);
                case DrawComponent::Shadows:  return _setShader(Cookable::CookType::Basic,    _camera, _lights, &_shadower);
                case DrawComponent::Geometry: return _setShader(Cookable::CookType::Geometry, _camera, {},      nullptr);
                case DrawComponent::Particle: return _setShader(Cookable::CookType::Particle, _camera, {},      nullptr);
            } return _placeHolder;
        }().set("diffuse_color", body.material));
    }
    //else {
    //    if (_userShadersName.find(type) == _userShadersName.cend())
    //        return;

    //    _UserShaderMemo& _usrShader = _userShaders[_userShadersName[type]];
    //    _usrShader.setter(model);
    //    model->draw(_usrShader.getter());
    //}
}

/*
#include "Renderer.hpp"

#include <iostream>
#include <algorithm>

#include "../../../Utils/Timer.hpp"
#include "../../../Utils/Physic/RayCaster.hpp"

// Rendering
void Renderer::quad(const Quad& surface) {
    if (!_deferred)
        return _drawQuadSync(surface);

    std::cerr << "[Warning] Renderer: can't draw quad in deferred mode. (Wrap these in a model and call draw)" << std::endl;
}

void Renderer::draw(Render::DrawType type, Model::Ref model) {
    if (!model)
        return;

    if (!_deferred)
        return _drawEntitySync(type, model);

    _DrawEntity di;
    di.type     = type;
    di.model    = model;
    di.drawId   = _heapEntities.size() + 1;
    di.priority = -1.0f;

    _heapEntities.emplace_back(std::move(di));
}

void Renderer::draw(const std::string& shaderName, Model::Ref model) {
    if (_userShaders.find(shaderName) == _userShaders.cend())
    {
        std::cerr << "Shader not found. Abort draw." << std::endl;
        return;
    }

    draw(_userShaders[shaderName].type, model);
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
void Renderer::_clear() {
    _heapEntities.clear();
    _heapText.clear();
}

void Renderer::_compute()
{
    // Apply batch transformations
    for (_DrawEntity& di : _heapEntities) {
        // Decide draw order:
        //  - opaque first: no order
        //  - transparent: furthest first
        di.priority = std::numeric_limits<float>::max();

        if (di.model->material.diffuse_color.a < 1.0f || di.model->material.force_reorder) {
            // Sort also poses in batch
            using dist_entity = std::tuple<float, Pose, Material>;
            std::vector<dist_entity> all_dist_entity;
            all_dist_entity.reserve(di.model->materials.size());

            size_t i_element = 0;
            auto& materials = di.model->materials;

            for (const Pose& pose : di.model->poses) {
                float entity_distance = RayCaster::ApproxDistance(_camera.position, di.model, pose);

                all_dist_entity.push_back({
                    entity_distance,
                    pose,
                    i_element < materials.size() ? materials[i_element] : Material{{0,0,0,0}}
                });

                di.priority = std::min(entity_distance, di.priority);

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

            di.model->poses = sorted_poses;
            di.model->materials = sorted_materials;
        }

        // Update model buffers
        di.model->_updateInternalBatch();
    }

    // Re-order vector from last to first
    std::sort(_heapEntities.begin(), _heapEntities.end(), [=](const _DrawEntity& di1, const _DrawEntity& di2) {
        return di1.priority > di2.priority;
    });

    // Shadows
    _shadower.render(_camera, _lights, [=](Shader& sh) {
        for (_DrawEntity& di : _heapEntities) {
            if (!di.model->material.cast_shadow)
                continue;

            if (di.priority < 0.0f)
                break;

            di.model->drawElements(sh);
        }
    });
}

void Renderer::_draw() {
    for (_DrawEntity& di : _heapEntities) {
        if (di.priority < 0.0f)
            break;

        _drawEntitySync(di.type, di.model, false);
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

void Renderer::_drawEntitySync(Render::DrawType type, Model::Ref model, bool update_buffer) {
    Shader placeHolder;

    if(update_buffer)
        model->_updateInternalBatch();

    if(type < Render::Custom) {
        model->draw([&]() -> Shader& {
            switch (type) {
                case Render::Basic:    return _setShader(Cookable::CookType::Basic,    _camera, {},      nullptr);
                case Render::Lights:   return _setShader(Cookable::CookType::Basic,    _camera, _lights, nullptr);
                case Render::Shadows:  return _setShader(Cookable::CookType::Basic,    _camera, _lights, &_shadower);
                case Render::Geometry: return _setShader(Cookable::CookType::Geometry, _camera, {},      nullptr);
                case Render::Particle: return _setShader(Cookable::CookType::Particle, _camera, {},      nullptr);
            } return placeHolder;
        }().set("diffuse_color", model->material.diffuse_color));
    }
    else {
        if (_userShadersName.find(type) == _userShadersName.cend())
            return;

        _UserShaderMemo& _usrShader = _userShaders[_userShadersName[type]];
        _usrShader.setter(model);
        model->draw(_usrShader.getter());
    }
}

void Renderer::_drawTextSync(const std::string& text, float x, float y, float scale, const glm::vec4& color) {
    TextEngine::Write(text, x, y, scale, color);
}
*/