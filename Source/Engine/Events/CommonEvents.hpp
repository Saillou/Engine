#pragma once

#include "Events.hpp"

// Define events available
struct CommonEvents : public Event {
	// -- Inputs --
	struct KeyPressed : public _Base {
		KeyPressed(int key = -1, int action = -1);
		int key;
		int action;
	};
	struct MouseMoved : public _Base {
		MouseMoved(int x = 0, int y = 0);
		int x;
		int y;
	};
	struct MouseButton : public _Base {
		MouseButton(int button = -1, int action = -1);
		int button;
		int action;
	};

	// -- State --
	struct StateUpdated : public _Base {
		StateUpdated();
	};

	// -- Scene --
	struct SceneRefresh : public _Base {
		SceneRefresh();
	};
	struct SceneEnded : public _Base {
		SceneEnded();
	};
	struct SceneFinishedRender : public _Base {
		SceneFinishedRender();
	};

	// -- Ray casting --
	struct MouseHit : public _Base {
		MouseHit(float x = 0, float y = 0, float z = 0);
		float x, y, z;
	};

private:
	enum _Type : int {
		EventKeyPressed,
		EventMouseMoved,
		EventMouseButton,
		
		EventStateUpdated,
		
		EventSceneEnded,
		EventSceneRefresh,
		EventSceneFinishedRender,
		
		EventMouseHit,
	};
};
