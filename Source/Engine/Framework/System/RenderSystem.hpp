#pragma once

#include <vector>

#include "../Core/System.hpp"
#include "../../Graphic/Camera.hpp"
#include "../../Graphic/Light.hpp"
#include "../../Graphic/Base/Model/Model.hpp"

struct RenderSystem : public System {
	RenderSystem(Camera& camera, std::vector<Light>& lights);
	~RenderSystem();

	void update(); // delta time too?

private:
	Camera& _camera;
	std::vector<Light>& _lights; // TODO: remove from here and define as components (?)
};