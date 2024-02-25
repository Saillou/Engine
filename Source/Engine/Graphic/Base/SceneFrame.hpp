#pragma once

#include <memory>

#include "Scene.hpp"
#include "Widget/Layout/Layout.hpp"

// Used for drawing UI on top of a Scene
struct SceneFrame : 
	protected Event::Subscriber
{
	SceneFrame(Scene& scene, std::shared_ptr<Layout> layout = nullptr);
	virtual ~SceneFrame();

	virtual void draw();
	Layout& layout();

private:
	Scene& m_scene;
	std::shared_ptr<Layout> m_layout;
};
