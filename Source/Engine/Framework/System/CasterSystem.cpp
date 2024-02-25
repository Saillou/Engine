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

void CasterSystem::update(const glm::vec2& mousePos, CastClosest cast_type)
{
    // Clear previous computations
    for (Entity entity : m_entities)
    {
        ECS::getComponent<CastComponent>(entity).is_hit = false;
    }

    switch (cast_type)
    {
        case CastClosest::Yes: 
            _flag_one(mousePos); 
            break;

        case CastClosest::No: 
        default: 
            _flag_all(mousePos); 
            break;
    }
}

void CasterSystem::_flag_all(const glm::vec2& mousePos) {
    for (Entity entity : m_entities)
    {
        const BodyComponent& body = ECS::getComponent<BodyComponent>(entity);
        CastComponent& cast = ECS::getComponent<CastComponent>(entity);

        auto cast_res = RayCaster::Intersect(mousePos, m_camera, body.model, body.transform.get());
        if (!cast_res)
            continue;

        cast.is_hit = true;
        cast.hit_position = glm::vec3(cast_res.value());
        cast.hit_distance = cast_res.value().w;
    }
}
void CasterSystem::_flag_one(const glm::vec2& mousePos) 
{
    Entity closest_entity_hit = (uint32_t) -1;
    std::optional<glm::vec4> closest_cast_res;

    for (Entity entity : m_entities)
    {
        const BodyComponent& body = ECS::getComponent<BodyComponent>(entity);
        CastComponent& cast = ECS::getComponent<CastComponent>(entity);

        auto cast_res = RayCaster::Intersect(mousePos, m_camera, body.model, body.transform.get());
        if (!cast_res)
            continue;

        if (!closest_cast_res.has_value() || closest_cast_res.value().w > cast_res.value().w) {
            closest_entity_hit = entity;
            closest_cast_res = cast_res;
        }
    }

    // Flag best fit
    if (closest_cast_res.has_value()) 
    {
        CastComponent& cast = ECS::getComponent<CastComponent>(closest_entity_hit);

        cast.is_hit = true;
        cast.hit_position = glm::vec3(closest_cast_res.value());
        cast.hit_distance = closest_cast_res.value().w;
    }
}
