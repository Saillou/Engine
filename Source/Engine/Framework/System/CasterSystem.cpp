#include "CasterSystem.hpp"

CasterSystem::CasterSystem()
{
}

void CasterSystem::init()
{
    Signature signature;

    signature.set(ECS::getComponentType<BodyComponent>());

    ECS::setSystemSignature<CasterSystem>(signature);
}

void CasterSystem::update()
{

}
