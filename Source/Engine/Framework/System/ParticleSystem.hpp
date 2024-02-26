#pragma once

#include "../Core/ECS.hpp"
#include "../Core/System.hpp"
#include "../Component/BodyComponent.hpp"
#include "../Component/ParticleComponent.hpp"

#include "../../Graphic/Camera.hpp"
#include "../../Graphic/ShaderManager.hpp"
#include "Render/Batch.hpp"

struct ParticleSystem : public System {
	ParticleSystem(const Camera&);
	~ParticleSystem() = default;

	void init() override;
	void update();

private:
	const Camera& _camera;

	std::unordered_map<CookType::_tag_, Batch> _batches;

	void _setSolidShader();
	void _setGeometryShader();
	void _setParticleShader();
};
