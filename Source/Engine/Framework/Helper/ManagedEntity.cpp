#include "ManagedEntity.hpp"

// Ctors
std::shared_ptr<ManagedEntity> ManagedEntity::Create(Model::Ref model) {
    struct _Managed_Entity_ : public ManagedEntity {
        _Managed_Entity_(Model::Ref model) : ManagedEntity(model) {};
    };
    return std::make_shared<_Managed_Entity_>(model);
}

ManagedEntity::ManagedEntity(Model::Ref model) :
    _entity(ECS::createEntity())
{
    BodyComponent body;
    body.model = model;

    DrawComponent draw;
    draw.type = DrawComponent::Solid;

    ECS::addComponent(_entity, body);
    ECS::addComponent(_entity, draw);
}

ManagedEntity::~ManagedEntity()
{
    ECS::destroyEntity(_entity);
}

// Entity id
Entity ManagedEntity::entity() const {
    return _entity;
}

// Features
void ManagedEntity::castable(bool enable)
{
    if (enable) {
        CastComponent casted;
        ECS::addComponent(_entity, casted);
    }
    else {
        ECS::removeComponent<CastComponent>(_entity);
    }
}

void ManagedEntity::collidable(bool enable)
{
    if (enable) {
        CollideComponent collider;
        ECS::addComponent(_entity, collider);
    }
    else {
        ECS::removeComponent<CollideComponent>(_entity);
    }
}

// Component
const BodyComponent& ManagedEntity::body() const{
    return ECS::getComponent<BodyComponent>(_entity);
}
BodyComponent& ManagedEntity::body() {
    return ECS::getComponent<BodyComponent>(_entity);
}

const DrawComponent& ManagedEntity::draw() const {
    return ECS::getComponent<DrawComponent>(_entity);
}
DrawComponent& ManagedEntity::draw() {
    return ECS::getComponent<DrawComponent>(_entity);
}
bool ManagedEntity::is_colliding() {
    if (!ECS::hasComponent<CollideComponent>(_entity))
        return false;

    return ECS::getComponent<CollideComponent>(_entity).is_colliding;
}

// Body component attributes
Model::Ref ManagedEntity::model() const {
    return body().model;
}
glm::mat4& ManagedEntity::local() {
    return body().transform.local;
}
glm::mat4& ManagedEntity::world() {
    return body().transform.world;
}
glm::mat4 ManagedEntity::transform() const{
    return ECS::getComponent<BodyComponent>(_entity).transform.get();
}

Material& ManagedEntity::material() {
    return body().material;
}
glm::vec4& ManagedEntity::color() {
    return body().material.color;
}
