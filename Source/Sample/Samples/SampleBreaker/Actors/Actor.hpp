#pragma once

#include <Engine/Framework/Helper/ManagedEntity.hpp>
#include <glm/glm.hpp>
#include <string>

struct Actor {
    Actor(Model::Ref model): 
        _entity_memory(ManagedEntity::Create(model)),
        _entity(*_entity_memory)
    { }

    virtual ~Actor() = default;

    virtual void setPos(const glm::vec3& pos) {
        _pos = pos;
        _entity.world() = glm::translate(glm::mat4(1.0f), _pos);
    }

    const glm::vec3& pos() const {
        return _pos;
    }

    Entity id() const {
        return _entity.entity();
    }

    ManagedEntity& entity() {
        return _entity;
    }

private:
    SharedEntity _entity_memory;

protected:
    ManagedEntity& _entity;

    glm::vec3 _pos = { 0, 0, 0 };

};
