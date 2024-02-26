#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(const Camera& camera):
    _camera(camera)
{
}

void ParticleSystem::init()
{
    Signature signature;

    signature.set(ECS::getComponentType<BodyComponent>());
    signature.set(ECS::getComponentType<ParticleComponent>());

    ECS::setSystemSignature<ParticleSystem>(signature);
}

void ParticleSystem::update()
{
    // Clear previous computation
    _batches.clear();

    // Create batches
    for (Entity entity : m_entities) 
    {
        // What kind of draw?
        const BodyComponent& body         = ECS::getComponent<BodyComponent>(entity);
        const ParticleComponent& particle = ECS::getComponent<ParticleComponent>(entity);

        CookType::_tag_ type = CookType::Custom;
        switch (particle.type)
        {
            case ParticleComponent::Type::Solid:    
                type = CookType::Solid; 
                break;

            case ParticleComponent::Type::Geometry:
                type = CookType::Geometry;
                break;

            case ParticleComponent::Type::Particle: 
                type = CookType::Particle; 
                break;

            default: 
                continue;
        }

        // Groups by type/model while computing batch
        for (const ParticleComponent::Element& elt : particle.elements) 
        {
            auto& curr_batch = _batches[type][body.model];

            curr_batch._entities.push_back(entity);
            curr_batch._materials.push_back(elt.color * body.material.color);
            curr_batch._transforms.push_back(elt.world * body.transform.get());
        }
    }

    // Prepare shading
    _setSolidShader();
    _setGeometryShader();
    _setParticleShader();

    // Draw
    for (const auto& itBatches : _batches)
    {
        CookType::_tag_ type = itBatches.first;
        const auto& batches  = itBatches.second;

        for (const auto& itBatch : batches)
        {
            Model::Ref model = itBatch.first;
            const _Batch& batch = itBatch.second;

            model->_setBatch(batch._transforms, batch._materials);
            model->draw(ShaderManager::Get(type));
        }
    }
}

// Shader stuff
void ParticleSystem::_setSolidShader()
{
    ShaderManager::Get(CookType::Solid)
        .use()
        .set("Projection",  _camera.projection)
        .set("View",        _camera.modelview)
        .set("CameraPos",   _camera.position)
        .set("far_plane",   _camera.far_plane)
        .set("use_shadow",  false)
        .set("LightNumber", 0)
    ;
}

void ParticleSystem::_setParticleShader() 
{
    ShaderManager::Get(CookType::Geometry)
        .use()
        .set("Projection", _camera.projection)
        .set("View",       _camera.modelview)
    ;
}

void ParticleSystem::_setGeometryShader() {
    ShaderManager::Get(CookType::Geometry)
        .use()
        .set("Projection",  _camera.projection)
        .set("View",        _camera.modelview)
        .set("n_lines",     2);
}