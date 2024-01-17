#include "CommonEvents.hpp"

// Public
// -- Keyboard events --
CommonEvents::KeyPressed::KeyPressed(int key) :
	_Base(_Type::EventKeyPressed),
	key(key)
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
