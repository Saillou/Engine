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
    _compute();

    Scene::Viewport(_shadower.width(), _shadower.height());
    _drawShadows();

    Scene::Viewport((int)_camera.screenSize.x, (int)_camera.screenSize.y);
    _drawEntities();
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
    // Clear previous compute
    _batches.clear();

    // Get info
    for (Entity entity : m_entities) {
        const DrawComponent&  draw  = ECS::getComponent<DrawComponent>(entity);
        const BodyComponent&  body  = ECS::getComponent<BodyComponent>(entity);

        if (draw.type == DrawComponent::Hidden)
            continue;

        _batches[body.model]._entities.push_back(entity);
        _batches[body.model]._transforms.push_back(body.transform);
        _batches[body.model]._materials.push_back(body.material);
    }

    // Set batches
    for (auto& it : _batches) {
        Model::Ref model    = it.first;
        const _Batch& batch = it.second;

        model->_setBatch(batch._transforms, batch._materials);
    }
}

// Draws
void RenderSystem::_drawShadows() 
{
    _shadower.render(_camera, _lights, [=](Shader& sh) {
        for (auto& it : _batches) {
            Model::Ref model = it.first;

            model->drawElements(sh);
        }
    });
}

void RenderSystem::_drawEntities()
{
    for (auto& it : _batches) {
        Model::Ref model = it.first;

        model->draw(sh);
    }
}

void RenderSystem::_drawEntity(Entity entity) 
{
    DrawComponent& draw = ECS::getComponent<DrawComponent>(entity);
    BodyComponent& body = ECS::getComponent<BodyComponent>(entity);

    if(draw.type < DrawComponent::Custom) {
        body.model->draw([&]() -> Shader& {
            switch (draw.type) {
                case DrawComponent::Basic:    return _setShader(Cookable::CookType::Basic,    _camera, {},      nullptr);
                case DrawComponent::Lights:   return _setShader(Cookable::CookType::Basic,    _camera, _lights, nullptr);
                case DrawComponent::Shadows:  return _setShader(Cookable::CookType::Basic,    _camera, _lights, &_shadower);
                case DrawComponent::Geometry: return _setShader(Cookable::CookType::Geometry, _camera, {},      nullptr);
                case DrawComponent::Particle: return _setShader(Cookable::CookType::Particle, _camera, {},      nullptr);
            } return _placeHolder;
        }());
    }
}
