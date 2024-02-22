#pragma once

#include <vector>
#include <memory>
#include <unordered_set>

#include "../Core/ECS.hpp"
#include "../Core/System.hpp"
#include "../Component/BodyComponent.hpp"
#include "../Component/DrawComponent.hpp"

#include "Render/ShadowRender.hpp"
#include "../../Graphic/TextEngine.hpp"
#include "../../Graphic/Wrapper/Framebuffer.hpp"
#include "../../Graphic/Camera.hpp"
#include "../../Graphic/Light.hpp"
#include "../../Graphic/Cookable.hpp"

struct RenderSystem : 
	public System, Cookable 
{
	RenderSystem(Camera& camera, std::vector<Light>& lights);
	~RenderSystem() = default;

	void init() override;
	void update(); // delta time too?

private:
	// Members
	Camera&				_camera;	// Given by Scene
	std::vector<Light>& _lights;	// Given by Scene
	ShadowRender		_shadower;
	Shader				_placeHolder;

	struct _Batch {
		std::vector<Entity>		_entities;
		std::vector<glm::mat4>	_transforms;
		std::vector<glm::vec4>	_materials;
	};
	std::unordered_map<Model::Ref, _Batch> _batches;

	// Helper
	Shader& _setShader(Cookable::CookType type, const Camera& camera, const std::vector<Light>& lights, const ShadowRender* shadower);

	// Prepare draw
	void _compute();

	// Actual draw calls
	void _drawShadows();
	void _drawEntities();
	void _drawEntity(Entity);
};
