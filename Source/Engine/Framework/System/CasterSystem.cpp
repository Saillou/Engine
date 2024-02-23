#include "CasterSystem.hpp"

CasterSystem::CasterSystem(const Camera& camera):
    m_camera(camera)
{
}

void CasterSystem::init()
{
    Signature signature;

    signature.set(ECS::getComponentType<BodyComponent>());
    signature.set(ECS::getComponentType<CastComponent>());

    ECS::setSystemSignature<CasterSystem>(signature);
}

void CasterSystem::update(const glm::vec2& mousePos)
{
    // Ray cast
    for (Entity entity : m_entities) 
    {
        const BodyComponent& body = ECS::getComponent<BodyComponent>(entity);
        CastComponent& cast = ECS::getComponent<CastComponent>(entity);

        cast.is_hit = false;

        auto cast_res = RayCaster::Intersect(mousePos, m_camera, body.model, body.transform.get());
        if (!cast_res)
            continue;

        cast.is_hit = true;
        cast.hit_position = glm::vec3(cast_res.value());
        cast.hit_distance = cast_res.value().w;
    }
}
