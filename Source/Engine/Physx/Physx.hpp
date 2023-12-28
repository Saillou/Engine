#pragma once

#include <memory>
#include <unordered_set>
#include <reactphysics3d/reactphysics3d.h>

#include "BaseBody.hpp"

// Singleton
class Physx {
public:
    typedef reactphysics3d::RigidBody* PBody;

    enum class BodyType {
        Static, Dynamic, Kinematic
    };

    static PBody Add(std::shared_ptr<BaseBody> body, BodyType type = BodyType::Static);
    static void Remove(PBody pbody);

    static void Clear();
    static void Compute(float delta_time_ms);

private:
    static Physx& _get();

    // Hide physics details
    struct _impl;
    std::unique_ptr<_impl> p_impl;

    // -- No copy --
    Physx();
    ~Physx();

    Physx& operator=(const Physx&) = delete;
    Physx(const Physx&) = delete;
    Physx(Physx&&) = delete;
};