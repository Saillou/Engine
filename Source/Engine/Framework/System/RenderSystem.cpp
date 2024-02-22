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
    // System requirements
    Signature signature;

    signature.set(ECS::getComponentType<BodyComponent>());
    signature.set(ECS::getComponentType<DrawComponent>());

    ECS::setSystemSignature<RenderSystem>(signature);

    // System members
    addRecipe(Cookable::CookType::Basic);
    addRecipe(Cookable::CookType::Geometry);
}

void RenderSystem::update()
{
    _compute();

    Scene::Viewport(_shadower.width(), _shadower.height());
    _drawShadows();

    Scene::Viewport((int)_camera.screenSize.x, (int)_camera.screenSize.y);
    _drawEntities();
}

// Prepare draw
void RenderSystem::_compute() 
{
    // Clear previous computations
    _batches.clear();
    _batches[CookType::Basic] = {};
    _batches[CookType::Geometry] = {};

    // Get info
    for (Entity entity : m_entities) {
        const DrawComponent& draw = ECS::getComponent<DrawComponent>(entity);
        const BodyComponent& body = ECS::getComponent<BodyComponent>(entity);

        if (draw.type == DrawComponent::Hidden)
            continue;

        CookType type = (CookType)-1;

        switch (draw.type)
        {
            case DrawComponent::Basic:
            case DrawComponent::Lights:
            case DrawComponent::Shadows:    
                type = CookType::Basic;    
                break;

            case DrawComponent::Geometry: 
                type = CookType::Geometry; 
                break;

            default: 
                continue;
        }

        _batches[type][body.model]._transforms.push_back(body.transform.get());
        _batches[type][body.model]._materials.push_back(body.material.color);
    }
}

// Draws
void RenderSystem::_drawShadows() 
{
    // Set batches
    const auto& batch_who_casts_shadows = _batches[CookType::Basic];

    for (auto& itBatch : batch_who_casts_shadows) {
        Model::Ref model    = itBatch.first;
        const _Batch& batch = itBatch.second;

        model->_setBatch(batch._transforms, batch._materials);
    }

    // Draw
    _shadower.render(_camera, _lights, [=](Shader& sh) {
        for (auto& it : batch_who_casts_shadows) {
            Model::Ref model = it.first;

            model->drawElements(sh);
        }
    });
}

void RenderSystem::_drawEntities()
{
    // Draw basic batches
    _setBasicShader(); 
    
    for (auto& itBatch : _batches[CookType::Basic]) {
        Model::Ref model = itBatch.first;
        _Batch& batch    = itBatch.second;

        // Batch (VBO instances) already setup thanks to shadow render
        model->draw(*get(CookType::Basic));
    }

    // Draw geometry batches
    _setGeometryShader();

    for (auto& itBatch : _batches[CookType::Geometry]) {
        Model::Ref model    = itBatch.first;
        const _Batch& batch = itBatch.second;

        model->_setBatch(batch._transforms, batch._materials);
        model->draw(*get(CookType::Geometry));
    }
}

// Shader stuff
void RenderSystem::_setBasicShader()
{
    // Check light capabilities
    if (get(Cookable::CookType::Basic)->source(ShaderSource::Type::Fragment).getVar("LightPos").count < _lights.size())
    {
        editRecipe(Cookable::CookType::Basic, ShaderSource::Type::Fragment, ShaderSource{}
            .add_var("uniform", "vec3", "LightPos", (int)_lights.size())
            .add_var("uniform", "vec4", "LightColor", (int)_lights.size())
            .add_var("uniform", "samplerCube", "depthMap", (int)_lights.size())
        );
    }

    // Use
    Shader& sh = get(Cookable::CookType::Basic)->use();

    // Bind shadow map
    _shadower.bindTextures(GL_TEXTURE0 + 1);

    // Set uniforms
    sh.set("Projection", _camera.projection)
      .set("View", _camera.modelview)
      .set("CameraPos", _camera.position)
      .set("far_plane", _camera.far_plane)
      .set("use_shadow", true)
      .set("LightNumber", (int)_lights.size());

    for (int iLight = 0; iLight < (int)_lights.size(); iLight++)
    {
        sh.set("LightPos[" +   std::to_string(iLight) + "]", _lights[iLight].position)
          .set("LightColor[" + std::to_string(iLight) + "]", _lights[iLight].color)
          .set("depthMap[" +   std::to_string(iLight) + "]", iLight + 1);
    }
}

void RenderSystem::_setGeometryShader() {
    Shader& sh = get(Cookable::CookType::Geometry)->use();

    sh.set("Projection", _camera.projection)
      .set("View",       _camera.modelview)
      .set("n_lines",    2);
}
