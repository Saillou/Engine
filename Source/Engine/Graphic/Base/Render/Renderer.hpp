#pragma once

#include "../../Camera.hpp"
#include "../../Cookable.hpp"
#include "../../Light.hpp"
#include "../../TextEngine.hpp"
#include "../../Utils/Framebuffer.hpp"
#include "../Model/Entity.hpp"
#include "../Render/RenderType.hpp"
#include "ShadowRender.hpp"

#include <vector>
#include <memory>

struct Renderer : private Cookable {
	friend struct BaseScene;

	// Methods
	void draw(Render::DrawType, Entity& entity);
	void text(const std::string& text, float x, float y, float scale = 0.5f, const glm::vec4& color = glm::vec4(1, 1, 1, 1));

private:
	Renderer() = default;

	Shader& _setShader(Cookable::CookType type, const Camera& camera, const std::vector<Light>& lights, const ShadowRender* shadower);

	Camera _camera				= {};
	std::vector<Light> _lights	= {};
	ShadowRender _shadower;
	bool _deferred = false;

	void _clear();
	void _compute();
	void _draw();

	struct _DrawEntity {
		Render::DrawType type;

		std::shared_ptr<Model> _model;

		Pose _localPose;
		Material _localMaterial;
		std::vector<Pose> _poses;
		std::vector<Material> _materials;
	};

	struct _DrawText {
		std::string text;
		float x;
		float y;
		float scale;
		glm::vec4 color;
	};

	std::vector<_DrawEntity> _heapEntities;
	std::vector<_DrawText> _heapText;
};