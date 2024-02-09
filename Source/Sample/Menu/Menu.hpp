#pragma once

#include <Engine/Graphic/Window.hpp>
#include <Engine/Events/Events.hpp>

#include <memory>

enum class EditorId
{
	None = 0,
	Material, Model, World
};

struct Menu : Event::Subscriber 
{
	Menu();
	~Menu();

private:
	void _prepare();
	void _render();

	// Members
	struct State
	{
		EditorId editorId = EditorId::None;
	} state;
};