#pragma once

#include <Engine/Framework/Helper/ManagedEntity.hpp>
#include <glm/glm.hpp>
#include <string>

// --------------------------- Base Actor ---------------------------
struct Actor {
    Actor(Model::Ref model): 
        _entity_memory(ManagedEntity::Create(model)),
        _entity(*_entity_memory)
    { }

    virtual ~Actor() { }

    const glm::vec3& pos() const {
        return _pos;
    }

    void setPos(const glm::vec3& pos) {
        _pos = pos;
        _entity.world() = glm::translate(glm::mat4(1.0f), _pos);
    }

    Entity id() const {
        return _entity.entity();
    }

    ManagedEntity& entity() {
        return _entity;
    }

protected:
    SharedEntity _entity_memory;
    ManagedEntity& _entity;
    glm::vec3 _pos = { 0, 0, 0 };
};
