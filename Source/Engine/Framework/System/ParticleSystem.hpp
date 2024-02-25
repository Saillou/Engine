#pragma once

#include "../Core/ECS.hpp"
#include "../Core/System.hpp"
#include "../Component/BodyComponent.hpp"
#include "../Component/ParticleComponent.hpp"

#include "../../Graphic/Camera.hpp"

struct ParticleSystem : public System {
	ParticleSystem(const Camera&);
	~ParticleSystem() = default;

	void init() override;
	void update(float dt_ms);

private:
	const Camera& m_camera;
};
