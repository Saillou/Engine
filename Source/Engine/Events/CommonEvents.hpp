#pragma once

#include "Events.hpp"

// Define events available
struct CommonEvents : public Event {
	// -- Inputs --
	struct KeyPressed : public _Base {
		explicit KeyPressed(int key = -1);
		int key;
	};
	struct MouseMoved : public _Base {
		explicit MouseMoved(int x = 0, int y = 0);
		int x;
		int y;
	};

	// -- State --
	struct StateUpdated : public _Base {
		explicit StateUpdated();
	};

	// -- Scene --
	struct SceneRefresh : public _Base {
		explicit SceneRefresh();
	};
	struct SceneEnded : public _Base {
		explicit SceneEnded();
	};

private:
	enum _Type : int {
		EventKeyPressed,
		EventMouseMoved,

		EventStateUpdated,

		EventSceneEnded,
		EventSceneRefresh,
	};
};
