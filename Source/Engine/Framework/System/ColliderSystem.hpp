#pragma once

#include <vector>
#include <memory>
#include <unordered_set>

#include "Physic/Collider.hpp"

#include "../Core/ECS.hpp"
#include "../Core/System.hpp"
#include "../Component/BodyComponent.hpp"
#include "../Component/CollideComponent.hpp"

struct ColliderSystem : public System {
	void init() override;
	void update(Entity entityChecked);
};
