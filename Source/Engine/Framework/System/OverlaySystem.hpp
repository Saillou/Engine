#pragma once

#include <vector>
#include <memory>
#include <unordered_set>

#include "CanvasSystem.hpp"
#include "../Core/ECS.hpp"
#include "../Core/System.hpp"
#include "../Component/BodyComponent.hpp"
#include "../Component/DrawComponent.hpp"

#include "Render/ShadowRender.hpp"
#include "../../Graphic/Light.hpp"
#include "../../Graphic/ShaderManager.hpp"

struct SceneFrame;

struct OverlaySystem :
	public System
{
	 OverlaySystem(const Camera&);
	~OverlaySystem() = default;

	void init() override;
	void update();

	// Todo: remove these 2 methods and use entities
	void add_frame(SceneFrame*);
	void remove_frame(SceneFrame*);
	// --

	void quad(const Quad& surface);
	void text(const std::string& text, float x, float y, float scale, const glm::vec4& color);

private:
	const Camera& m_camera;

	// frames to render
	std::shared_ptr<CanvasSystem> _canvas_system;
	std::unordered_set<SceneFrame*> _frames;
};