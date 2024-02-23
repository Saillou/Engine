#pragma once

#include <vector>
#include <memory>
#include <unordered_set>

#include "Physic/RayCaster.hpp"

#include "../Core/ECS.hpp"
#include "../Core/System.hpp"
#include "../Component/BodyComponent.hpp"
#include "../Component/CastComponent.hpp"

#include "../../Graphic/Camera.hpp"

struct CasterSystem : public System {
	CasterSystem(const Camera&);
	~CasterSystem() = default;

	void init() override;
	void update(const glm::vec2& mousePos);

private:
	const Camera& m_camera;
};
