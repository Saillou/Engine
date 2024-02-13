#pragma once

#include "../../_imgui/imgui.h"
#include <Engine/Graphic/Window.hpp>

#include <unordered_map>
#include <vector>

struct UiBreaker
{
	UiBreaker() {};
	~UiBreaker() {};

	void show();

	bool show_debug = false;
	bool stop_time  = false;
	bool want_restart = false;
};