#pragma once

#include <Engine/Framework/Service.hpp>
#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Window.hpp>
#include <Engine/Utils/Timer.hpp>

enum class SampleId {
	Cube,		Train,		Forest, 
	Particles,	Mandala,	Pong,
	Breaker,	Snow
};

struct Sample : Event::Subscriber {
	virtual ~Sample() {
		_unsubscribeAll();
	}

	bool wantQuit = false;
};
