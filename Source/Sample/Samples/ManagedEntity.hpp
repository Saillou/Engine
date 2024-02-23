#pragma once

#include <iostream>
#include <string>

#include <glm/glm.hpp>

#include <Engine/Framework/Core/ECS.hpp>
#include <Engine/Framework/Component/BodyComponent.hpp>
#include <Engine/Framework/Component/CastComponent.hpp>
#include <Engine/Framework/Component/DrawComponent.hpp>

struct ManagedEntity 
{
    // Life cycle
    ManagedEntity(Model::Ref model):
        _entity(ECS::createEntity())
    { 
        BodyComponent body;
        body.model = model;

        DrawComponent draw;
        draw.type = DrawComponent::Solid;

        ECS::addComponent(_entity, body);
        ECS::addComponent(_entity, draw);
    }

    ~ManagedEntity() 
    {
        ECS::destroyEntity(_entity);
    }

    static std::shared_ptr<ManagedEntity> Create(Model::Ref model) {
        return std::make_shared<ManagedEntity>(model);
    }

    // Entity id
    Entity entity() const {
        return _entity;
    }

    // Features
    void castable(bool enable) 
    {
        if (enable) {
            CastComponent casted;
            ECS::addComponent(_entity, casted);
        }
        else {
            ECS::removeComponent<CastComponent>(_entity);
        }
    }

    // Component
    const BodyComponent& body() const{
        return ECS::getComponent<BodyComponent>(_entity);
    }
    BodyComponent& body() {
        return ECS::getComponent<BodyComponent>(_entity);
    }

    const DrawComponent& draw() const {
        return ECS::getComponent<DrawComponent>(_entity);
    }
    DrawComponent& draw() {
        return ECS::getComponent<DrawComponent>(_entity);
    }

    // Body component attributes
    Model::Ref model() const {
        return body().model;
    }
    glm::mat4& local() {
        return body().transform.local;
    }
    glm::mat4& world() {
        return body().transform.world;
    }
    const glm::mat4& transform() const{
        return ECS::getComponent<BodyComponent>(_entity).transform.get();
    }
    glm::vec4& color() {
        return body().material.color;
    }


private:
    Entity _entity;
};
typedef std::shared_ptr<ManagedEntity> SharedEntity;