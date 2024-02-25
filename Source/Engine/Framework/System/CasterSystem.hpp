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

struct CasterSystem : public System 
{
	// Just to piss of my mate
	enum class CastClosest {
		No, Yes
	};

	CasterSystem(const Camera&);
	~CasterSystem() = default;

	void init() override;
	void update(const glm::vec2& mousePos, CastClosest = CastClosest::No);

private:
	const Camera& m_camera;

	void _flag_all(const glm::vec2& mousePos);
	void _flag_one(const glm::vec2& mousePos);
};
