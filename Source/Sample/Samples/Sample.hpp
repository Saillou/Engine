#pragma once

#include <Engine/Events/CommonEvents.hpp>

#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Window.hpp>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Utils/Service.hpp>

enum class SampleId {
	Cube,		Train,		Forest, 
	Particles,	Mandala,	Pong
};

struct Sample : Event::Subscriber {
	virtual ~Sample() {
		_unsubscribeAll();
	}

	bool wantQuit = false;
};
