#include "RenderSystem.hpp"

#include "../../Graphic/Base/Scene.hpp"
#include "../../Utils/Timer.hpp"

#include <iostream>
#include <algorithm>

RenderSystem::RenderSystem(const Camera& camera, const std::vector<Light>& lights) :
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
}

void RenderSystem::update()
{
    _clear();
    _compute();

    Scene::Viewport(_shadower.width(), _shadower.height());
    _drawShadows();

    Scene::Viewport((int)_camera.screenSize.x, (int)_camera.screenSize.y);
    _drawEntities();
}

void RenderSystem::_clear()
{
    glClearColor(0.05f, 0.05f, 0.06f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Prepare draw
void RenderSystem::_compute() 
{
    // Clear previous computations
    _shadow_batch.clear();
    _batches_opaque.clear();
    _batches_translucent.clear();
    _custom_tags_apply.clear();

    // Get info
    for (Entity entity : m_entities) {
        // What kind of draw?
        const DrawComponent& draw = ECS::getComponent<DrawComponent>(entity);

        std::vector<CookType::_tag_> cooktypes;
        if (draw.type & DrawComponent::Solid)
            cooktypes.push_back(CookType::_tag_::Solid);

        if (draw.type & DrawComponent::Geometry)
            cooktypes.push_back(CookType::_tag_::Geometry);

        if (draw.type & DrawComponent::Custom) {
            CookType::_tag_ customTag = CookType::_tag_(draw.type >> DrawComponent::Custom);
            
            cooktypes.push_back(customTag);
            _custom_tags_apply.push_back(customTag);
        }

        if (cooktypes.empty())
            continue;

        // Fetch bodies
        const BodyComponent& body = ECS::getComponent<BodyComponent>(entity);

        // Shadow bodies
        if (body.material.cast_shadow) {
            // Only need the transform (no material used in shadows)
            _shadow_batch[body.model]._transforms.push_back(body.transform.get());
        }

        // Drawn bodies
        for (CookType::_tag_ type : cooktypes)
        {
            bool need_reorder = body.material.color.a < 1.0f;
            if (need_reorder) {
                // Only need the entity, other values will be fetched after re-order
                _batches_translucent[type][body.model]._entities.push_back(entity);
            }
            else {
                // Normal batches
                _batches_opaque[type][body.model]._materials.push_back(body.material.color);
                _batches_opaque[type][body.model]._transforms.push_back(body.transform.get());
            }
        }
    }

    // Need to reorder translucent elements
    for (auto& itBatches : _batches_translucent) {
        for (auto& itBatch : itBatches.second) {
            _reorder(itBatch.first, itBatch.second);
        }
    }
}

void RenderSystem::_reorder(Model::Ref model, _Batch& batch)
{
    std::unordered_map<Entity, float> cache_distance;

    // Sort entities by distance to the camera
    std::sort(batch._entities.begin(), batch._entities.end(), [&](const Entity& e1, const Entity& e2) 
    {
        if (cache_distance.find(e1) == cache_distance.cend()) {
            const glm::mat4& quat = ECS::getComponent<BodyComponent>(e1).transform.get();
            cache_distance[e1] = RayCaster::ApproxDistance(_camera.position, model, quat);
        }
        if (cache_distance.find(e2) == cache_distance.cend()) {
            const glm::mat4& quat = ECS::getComponent<BodyComponent>(e2).transform.get();
            cache_distance[e2] = RayCaster::ApproxDistance(_camera.position, model, quat);
        }

        return cache_distance.at(e1) > cache_distance.at(e2);
    });

    // Fetch entities
    for (Entity entity : batch._entities) 
    {
        const BodyComponent& body = ECS::getComponent<BodyComponent>(entity);

        batch._materials.push_back(body.material.color);
        batch._transforms.push_back(body.transform.get());
    }

}

// Draws
void RenderSystem::_drawShadows() 
{
    // Set batches
    for (auto& itBatch : _shadow_batch) {
        Model::Ref model    = itBatch.first;
        const _Batch& batch = itBatch.second;

        model->_setBatch(batch._transforms);
    }

    // Draw
    _shadower.render(_camera, _lights, [=](Shader& sh) {
        for (auto& it : _shadow_batch) {
            Model::Ref model = it.first;

            model->drawElements(sh);
        }
    });
}

void RenderSystem::_drawEntities()
{
    // Setup shaders
    _setSolidShader();
    _setGeometryShader();

    for (CookType::_tag_ tag : _custom_tags_apply) {
        ShaderManager::Apply(tag);
    }
    
    // Orders matter
    for (const auto& batches : { _batches_opaque , _batches_translucent })
    {
        for (const auto& itBatches : batches)
        {
            CookType::_tag_ type = itBatches.first;
            const auto& batches  = itBatches.second;

            for (const auto& itBatch : batches) 
            {
                Model::Ref model    = itBatch.first;
                const _Batch& batch = itBatch.second;

                model->_setBatch(batch._transforms, batch._materials);
                model->draw(ShaderManager::Get(type));
            }
        }
    }
}

// Shader stuff
void RenderSystem::_setSolidShader()
{
    // Check light capabilities
    if (ShaderManager::Get(CookType::Solid)
        .source(ShaderSource::Type::Fragment)
        .getVar("LightPos").count < _lights.size())
    {
        ShaderManager::Edit(CookType::Solid, ShaderSource::Type::Fragment, ShaderSource{}
            .add_var("uniform", "vec3", "LightPos", (int)_lights.size())
            .add_var("uniform", "vec4", "LightColor", (int)_lights.size())
            .add_var("uniform", "samplerCube", "depthMap", (int)_lights.size())
        );
    }

    // Use
    Shader& sh = ShaderManager::Get(CookType::Solid)
        .use();

    // Bind shadow map
    _shadower.bindTextures(GL_TEXTURE0 + 1);

    // Set uniforms
    sh.set("Projection",  _camera.projection)
      .set("View",        _camera.modelview)
      .set("CameraPos",   _camera.position)
      .set("far_plane",   _camera.far_plane)
      .set("use_shadow",  true)
      .set("LightNumber", (int)_lights.size())
    ;

    for (int iLight = 0; iLight < (int)_lights.size(); iLight++)
    {
        sh.set("LightPos[" +   std::to_string(iLight) + "]", _lights[iLight].position)
          .set("LightColor[" + std::to_string(iLight) + "]", _lights[iLight].color)
          .set("depthMap[" +   std::to_string(iLight) + "]", iLight + 1);
    }
}

void RenderSystem::_setGeometryShader() {
    ShaderManager::Get(CookType::Geometry)
        .use()
        .set("Projection", _camera.projection)
        .set("View",       _camera.modelview)
        .set("n_lines",    2);
}
