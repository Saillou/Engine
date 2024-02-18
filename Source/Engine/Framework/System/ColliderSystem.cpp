#include "ColliderSystem.hpp"

ColliderSystem::ColliderSystem()
{
}

void ColliderSystem::init()
{
    Signature signature;

    signature.set(ECS::getComponentType<BodyComponent>());

    ECS::setSystemSignature<ColliderSystem>(signature);
}

void ColliderSystem::update()
{

}
