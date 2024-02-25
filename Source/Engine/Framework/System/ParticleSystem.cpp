#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(const Camera& camera):
    m_camera(camera)
{
}

void ParticleSystem::init()
{
    Signature signature;

    signature.set(ECS::getComponentType<BodyComponent>());
    signature.set(ECS::getComponentType<ParticleComponent>());

    ECS::setSystemSignature<ParticleSystem>(signature);
}

void ParticleSystem::update(float dt_ms)
{
    for (Entity entity : m_entities) 
    {
        // ..
    }
}
