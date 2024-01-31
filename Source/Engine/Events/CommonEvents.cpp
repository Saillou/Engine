#include "CommonEvents.hpp"

// Public
// -- Keyboard events --
CommonEvents::KeyPressed::KeyPressed(int key, int action) :
	_Base(_Type::EventKeyPressed),
	key(key),
	action(action)
{
	// ..
}

// -- Mouse events --
CommonEvents::MouseMoved::MouseMoved(int x, int y) :
	_Base(_Type::EventMouseMoved),
	x(x), y(y)
{
	// ..
}
CommonEvents::MouseButton::MouseButton(int button, int action, int x, int y) :
	_Base(_Type::EventMouseButton),
	button(button),
	action(action),
	x(x), y(y)
{
	// ..
}


// -- Game events --
CommonEvents::StateUpdated::StateUpdated() :
	_Base(_Type::EventStateUpdated)
{
	// ..
}

CommonEvents::SceneEnded::SceneEnded() :
	_Base(_Type::EventSceneEnded)
{
	// ..
}

CommonEvents::SceneRefresh::SceneRefresh() :
	_Base(_Type::EventSceneRefresh)
{
	// ..
}

CommonEvents::SceneFinishedRender::SceneFinishedRender() :
				_Base(_Type::EventSceneFinishedRender)
{
				// ..
}

CommonEvents::MouseHit::MouseHit(float x, float y, float z) :
				_Base(_Type::EventMouseHit),
				x(x), y(y), z(z)
{
}
