#pragma once

#include <vector>
#include <memory>
#include <unordered_set>

#include "../Core/ECS.hpp"
#include "../Core/System.hpp"
#include "../Component/BodyComponent.hpp"
#include "../Component/DrawComponent.hpp"

#include "Render/Batch.hpp"
#include "Render/ShadowRender.hpp"
#include "../../Graphic/Camera.hpp"
#include "../../Graphic/Light.hpp"
#include "../../Graphic/ShaderManager.hpp"

struct RenderSystem : 
	public System 
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

	Batch _shadow_batch;
	std::unordered_map<CookType::_tag_, Batch> _batches_opaque;
	std::unordered_map<CookType::_tag_, Batch> _batches_translucent;
	std::vector<CookType::_tag_> _custom_tags_apply;

	// Prepare draw
	void _clear();
	void _compute();
	void _reorder(Model::Ref, _Batch&);

	void _setSolidShader();
	void _setGeometryShader();

	// Actual draw calls
	void _drawShadows();
	void _drawEntities();
};
