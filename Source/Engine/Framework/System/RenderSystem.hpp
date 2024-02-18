#pragma once

#include <vector>
#include <memory>
#include <unordered_set>

#include "../Core/ECS.hpp"
#include "../Core/System.hpp"
#include "../Component/RenderComponent.hpp"

#include "ShadowRender.hpp"
#include "../../Graphic/TextEngine.hpp"
#include "../../Graphic/Wrapper/Framebuffer.hpp"
#include "../../Graphic/Camera.hpp"
#include "../../Graphic/Light.hpp"
#include "../../Graphic/Cookable.hpp"

struct RenderSystem : public System, Cookable {
	RenderSystem(Camera& camera, std::vector<Light>& lights);
	~RenderSystem() = default;

	void update(); // delta time too?

private:
	// Members
	Camera&				_camera;	// Given by Scene
	std::vector<Light>& _lights;	// Given by Scene
	ShadowRender		_shadower;
	Shader				_placeHolder;

	// Helper
	Shader& _setShader(Cookable::CookType type, const Camera& camera, const std::vector<Light>& lights, const ShadowRender* shadower);

	// Prepare draw
	//void _clear();
	void _compute();

	// Actual draw calls
	//void _draw();
	void _drawShadows();
	void _drawEntity(Entity);

	//// Deferred rendering
	//struct _DrawEntity {
	//	size_t drawId;
	//	float priority;
	//	RenderComponent::RenderType type;
	//	Model::Ref model = nullptr;
	//};

	//struct _DrawText {
	//	std::string text;
	//	float x;
	//	float y;
	//	float scale;
	//	glm::vec4 color;
	//};

	//std::vector<_DrawEntity> _heapEntities;
	//std::vector<_DrawText>	 _heapText;

	//// External shaders
	//struct _UserShaderMemo {
	//	RenderComponent::RenderType type; // >= DrawType::Custom
	//	ShaderGetter getter;
	//	ShaderSetter setter;
	//};
	//std::unordered_map<Render::DrawType, std::string> _userShadersName;
	//std::unordered_map<std::string, _UserShaderMemo> _userShaders;
};

/*
#pragma once

#include "../../Camera.hpp"
#include "../../Cookable.hpp"
#include "../../Light.hpp"
#include "../../TextEngine.hpp"
#include "../../Wrapper/Framebuffer.hpp"
#include "../Model/Model.hpp"
#include "../Render/RenderType.hpp"
#include "ShadowRender.hpp"

#include <vector>
#include <memory>
#include <unordered_set>

struct Renderer : private Cookable {
	friend struct Scene;

	// Render
	void quad(const Quad& surface);
	void draw(Render::DrawType, Model::Ref model);
	void draw(const std::string& shaderName, Model::Ref model);
	void text(const std::string& text, float x, float y, float scale = 0.5f, const glm::vec4& color = glm::vec4(1, 1, 1, 1));

	// Methods
	typedef std::function<Shader& (void)>	    ShaderGetter;
	typedef std::function<void(const Model::Ref)> ShaderSetter;

	void add_shader(const std::string& shaderName, const ShaderGetter&, const ShaderSetter&);
	void remove_shader(const std::string& shaderName);

private:
	Renderer() = default;

	Shader& _setShader(Cookable::CookType type, const Camera& camera, const std::vector<Light>& lights, const ShadowRender* shadower);

	bool _deferred				= true;
	Camera _camera				= {};
	std::vector<Light> _lights	= {};
	ShadowRender _shadower;

	void _clear();
	void _compute();
	void _draw();

	void _drawQuadSync(const Quad& surface);
	void _drawEntitySync(Render::DrawType, Model::Ref model, bool update_buffer = true);
	void _drawTextSync(const std::string& text, float x, float y, float scale = 0.5f, const glm::vec4& color = glm::vec4(1, 1, 1, 1));

	// Deferred rendering
	struct _DrawEntity {
		size_t drawId;
		float priority;
		Render::DrawType type;
		Model::Ref model = nullptr;
	};

	struct _DrawText {
		std::string text;
		float x;
		float y;
		float scale;
		glm::vec4 color;
	};

	std::vector<_DrawEntity> _heapEntities;
	std::vector<_DrawText>	 _heapText;

	// External shaders
	struct _UserShaderMemo {
		Render::DrawType type; // >= DrawType::Custom
		ShaderGetter getter;
		ShaderSetter setter;
	};
	std::unordered_map<Render::DrawType, std::string> _userShadersName;
	std::unordered_map<std::string, _UserShaderMemo> _userShaders;
};
*/