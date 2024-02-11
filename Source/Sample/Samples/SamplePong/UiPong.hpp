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

private:
	bool _show_debug = true;
	bool _stop_time = false;
};