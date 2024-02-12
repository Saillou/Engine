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

// -- State events --
CommonEvents::StateUpdated::StateUpdated() :
	_Base(_Type::EventStateUpdated)
{
	// ..
}

