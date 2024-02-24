#pragma once

#include <Engine/Graphic/Window.hpp>
#include <Engine/Events/Events.hpp>

#include <memory>

struct AppMenu : Event::Subscriber
{
	AppMenu();
	~AppMenu();

private:
	void _prepare();
	void _render();

	bool _frame_ready = false;
};