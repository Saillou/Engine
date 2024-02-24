#pragma once

/*
    Transform C-style entity to C++ OOP

    Target: 
        - be able to forget about destroy()
        - simpler syntax for accessing data
        - basic components already defined

    It's just an helper, you don't have to use it.
*/

#include <iostream>
#include <string>

#include <glm/glm.hpp>

#include "../Core/ECS.hpp"
#include "../Component/BodyComponent.hpp"
#include "../Component/CastComponent.hpp"
#include "../Component/DrawComponent.hpp"
#include "../Component/CollideComponent.hpp"

struct ManagedEntity 
{
    // Ctors
    static std::shared_ptr<ManagedEntity> Create(Model::Ref model);
    virtual ~ManagedEntity();

    // Entity id
    Entity entity() const;

    // Features
    void castable(bool enable);
    void collidable(bool enable);

    // Component
    const BodyComponent& body() const;
    BodyComponent& body();

    const DrawComponent& draw() const;
    DrawComponent& draw();
    bool is_colliding();

    // Body component attributes
    Model::Ref model() const;
    glm::mat4& local();
    glm::mat4& world();
    glm::mat4 transform() const;

    Material& material();
    glm::vec4& color();

protected:
    // Life cycle
    ManagedEntity(Model::Ref model);

private:
    Entity _entity;
};
typedef std::shared_ptr<ManagedEntity> SharedEntity;