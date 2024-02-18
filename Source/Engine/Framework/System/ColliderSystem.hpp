#pragma once

#include <vector>
#include <memory>
#include <unordered_set>

#include "Physic/Collider.hpp"

#include "../Core/ECS.hpp"
#include "../Core/System.hpp"
#include "../Component/BodyComponent.hpp"

struct ColliderSystem : public System {
	ColliderSystem();
	~ColliderSystem() = default;

	void init() override;
	void update(); // delta time too?
};
