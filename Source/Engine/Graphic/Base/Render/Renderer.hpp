#pragma once

#include "../../Camera.hpp"
#include "../../Cookable.hpp"
#include "../../Light.hpp"
#include "../../TextEngine.hpp"
#include "../../Wrapper/Framebuffer.hpp"
#include "../Model/Primitive/Quad.hpp"
#include "../Model/Entity.hpp"
#include "../Render/RenderType.hpp"
#include "ShadowRender.hpp"

#include <vector>
#include <memory>
#include <unordered_set>

struct Renderer : private Cookable {
	friend struct Scene;

	// Render
	void quad(const Quad& surface);
	void draw(Render::DrawType, Entity& entity);
	void draw(const std::string& shaderName, Entity& entity);
	void text(const std::string& text, float x, float y, float scale = 0.5f, const glm::vec4& color = glm::vec4(1, 1, 1, 1));

	// Methods
	typedef std::function<Shader& (void)>				ShaderGetter;
	typedef std::function<void(const Entity& entity)>	ShaderSetter;

	void add_shader(const std::string& shaderName, const ShaderGetter&, const ShaderSetter&);

private:
	Renderer() = default;

	Shader& _setShader(Cookable::CookType type, const Camera& camera, const std::vector<Light>& lights, const ShadowRender* shadower);

	bool _deferred = true;
	Camera _camera				= {};
	std::vector<Light> _lights	= {};
	ShadowRender _shadower;

	void _clear();
	void _compute();
	void _draw();

	void _drawQuadSync(const Quad& surface);
	void _drawEntitySync(Render::DrawType, Entity& entity, bool update_buffer = true);
	void _drawTextSync(const std::string& text, float x, float y, float scale = 0.5f, const glm::vec4& color = glm::vec4(1, 1, 1, 1));

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

	std::vector<_DrawEntity>	_heapEntities;
	std::vector<_DrawText>		_heapText;
	std::unordered_set<size_t>	_entitiesDuplicates;

	// External shaders
	struct _UserShaderMemo {
		Render::DrawType type; // >= DrawType::Custom
		ShaderGetter getter;
		ShaderSetter setter;
	};
	std::unordered_map<Render::DrawType /*type*/, std::string /*name*/> _userShadersName;
	std::unordered_map<std::string /*name*/, _UserShaderMemo> _userShaders;
};