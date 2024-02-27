#include "CanvasSystem.hpp"

#include "../Component/CanvasComponent.hpp"

CanvasSystem::CanvasSystem(const Camera& camera) :
	m_camera(camera)
{
}

void CanvasSystem::init()
{
    Signature signature;

    signature.set(ECS::getComponentType<CanvasComponent>());

    ECS::setSystemSignature<CanvasSystem>(signature);
}

void CanvasSystem::draw()
{
    for (Entity entity : m_entities) 
    {
        const CanvasComponent& cnvComponent = ECS::getComponent<CanvasComponent>(entity);

    }
}
