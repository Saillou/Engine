#pragma once

#include "../../Camera.hpp"
#include "../../Cookable.hpp"
#include "../../Light.hpp"
#include "../../TextEngine.hpp"
#include "../../Utils/Framebuffer.hpp"
#include "../Model/Entity.hpp"
#include "../Render/RenderType.hpp"
#include "ShadowRender.hpp"

struct Renderer : private Cookable {
	friend struct BaseScene;

	// Methods
	void draw(Render::DrawType, const Entity& entity);
	void text(const std::string& text, float x, float y, float scale, const glm::vec4& color);

private:
	Renderer();

	Shader& _setShader(Cookable::CookType type, const Camera& camera, const std::vector<Light>& lights, const ShadowRender* shadower);

	Camera _camera				= {};
	std::vector<Light> _lights	= {};
	ShadowRender _shadower;
};