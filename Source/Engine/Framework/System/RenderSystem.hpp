#pragma once

#include <vector>
#include <memory>
#include <unordered_set>

#include "../Core/ECS.hpp"
#include "../Core/System.hpp"
#include "../Component/BodyComponent.hpp"
#include "../Component/DrawComponent.hpp"

#include "Render/ShadowRender.hpp"
#include "../../Graphic/Camera.hpp"
#include "../../Graphic/Light.hpp"
#include "../../Graphic/Cookable.hpp"

struct RenderSystem : 
	public System, Cookable 
{
	RenderSystem(const Camera& camera, const std::vector<Light>& lights);
	~RenderSystem() = default;

	void init() override;
	void update(); // delta time too?

private:
	// Members
	const Camera&			  _camera;	// Given by Scene
	const std::vector<Light>& _lights;	// Given by Scene
	ShadowRender			  _shadower;

	struct _Batch {
		std::vector<Entity>     _entities;
		std::vector<glm::mat4>	_transforms;
		std::vector<glm::vec4>	_materials;
	};
	typedef std::unordered_map<Model::Ref, _Batch> Batch;

	Batch _shadow_batch;
	std::unordered_map<CookType, Batch> _batches_opaque;
	std::unordered_map<CookType, Batch> _batches_translucent;

	// Prepare draw
	void _compute();
	void _reorder(Model::Ref, _Batch&);

	void _setSolidShader();
	void _setGeometryShader();

	// Actual draw calls
	void _drawShadows();
	void _drawEntities();
};
