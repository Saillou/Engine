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
#include <unordered_set>

struct Renderer : private Cookable {
	friend struct Scene;

	// Methods
	void draw(Render::DrawType, Entity& entity);
	void text(const std::string& text, float x, float y, float scale = 0.5f, const glm::vec4& color = glm::vec4(1, 1, 1, 1));
	bool deferred = true;

private:
	Renderer() = default;

	Shader& _setShader(Cookable::CookType type, const Camera& camera, const std::vector<Light>& lights, const ShadowRender* shadower);

	Camera _camera				= {};
	std::vector<Light> _lights	= {};
	ShadowRender _shadower;

	void _clear();
	void _compute();
	void _draw();

	void _drawEntitySync(Render::DrawType, Entity& entity, bool update_buffer = true);

	// Deferred rendering
	struct _DrawEntity {
		size_t drawId;
		float drawPriority;
		Render::DrawType type;
		Entity copied_entity;
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
	std::unordered_set<size_t> _entitiesDuplicates;
};