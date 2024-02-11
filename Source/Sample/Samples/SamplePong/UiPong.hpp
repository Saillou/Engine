#pragma once

#include "../../_imgui/imgui.h"
#include <Engine/Graphic/Window.hpp>

#include <unordered_map>
#include <vector>

struct UiPong
{
	UiPong() {};
	~UiPong() {};

	void show();

	bool show_debug = true;
	bool stop_time  = false;
	bool want_restart = false;
};