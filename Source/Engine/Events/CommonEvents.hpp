#pragma once

#include "Events.hpp"

// Define events available
struct CommonEvents : public Event {
	// -- Inputs --
	struct KeyPressed : public _Base {
		KeyPressed(int key = -1, int action = -1) :
			_Base(_Type::EventKeyPressed), key(key), action(action)
		{ }

		int key;
		int action;
	};
	struct MouseMoved : public _Base {
		MouseMoved(int x = 0, int y = 0) :
			_Base(_Type::EventMouseMoved), x(x), y(y)
		{ }

		int x;
		int y;
	};
	struct MouseButton : public _Base {
		MouseButton(int button = -1, int action = -1, int x = -1, int y = -1) :
			_Base(_Type::EventMouseButton), button(button), action(action), x(x), y(y)
		{ }

		int button;
		int action;
		int x;
		int y;
	};

	// -- State --
	struct StateUpdated : public _Base {
		StateUpdated() :
			_Base(_Type::EventStateUpdated)
		{ }
	};

private:
	enum _Type : int {
		EventKeyPressed,
		EventMouseMoved,
		EventMouseButton,

		EventStateUpdated,
	};
};
