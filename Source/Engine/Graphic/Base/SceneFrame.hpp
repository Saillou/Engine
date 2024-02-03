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
	Scene& scene();

private:
	void _on_draw(const SceneEvents::RenderFinished&);

	Framebuffer m_frame;
	CopyFilter m_copyFilter;
	Scene& m_scene;
	std::shared_ptr<Layout> m_layout;
};
