#pragma once

#include <memory>

#include "Scene.hpp"
#include "Widget/Layout/Layout.hpp"

// Used for drawing UI on top of a Scene
struct SceneFrame : 
	protected Event::Subscriber
{
	SceneFrame(Scene& scene, std::shared_ptr<Layout> layout = nullptr);
	virtual ~SceneFrame() = default;

	Layout& layout();

private:
	void _on_draw(const SceneEvents::RenderFinished&);

	Scene& m_scene;
	std::shared_ptr<Layout> m_layout;
};
