#pragma once

#include <vector>
#include <memory>
#include <unordered_set>

#include "Physic/RayCaster.hpp"

#include "../Core/ECS.hpp"
#include "../Core/System.hpp"
#include "../Component/BodyComponent.hpp"

struct CasterSystem : public System {
	CasterSystem();
	~CasterSystem() = default;

	void init() override;
	void update(); // delta time too?
};
