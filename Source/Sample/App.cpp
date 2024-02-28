#include "App.hpp"

// List of samples
#include "Samples/SampleCube/SampleCube.hpp"
#include "Samples/SampleTrain/SampleTrain.hpp"
#include "Samples/SampleForest/SampleForest.hpp"
#include "Samples/SampleSnow/SampleSnow.hpp"
#include "Samples/SampleBreaker/SampleBreaker.hpp"
#include "Samples/SampleCanvas/SampleCanvas.hpp"

// App
App::App():
    m_curr_sample(std::make_unique<SampleCanvas>())
{
}

void App::changeSample(const SampleId& sample_id) {
	m_curr_sample = nullptr;

	switch (sample_id)
	{
		case SampleId::Cube:	m_curr_sample = std::make_unique<SampleCube>();		break; // Layout UI, Opacity reorder, Multi lights
		case SampleId::Train:	m_curr_sample = std::make_unique<SampleTrain>();	break; // Textured quads, Camera movement
		case SampleId::Forest:	m_curr_sample = std::make_unique<SampleForest>();	break; // Ray caster
		case SampleId::Snow:	m_curr_sample = std::make_unique<SampleSnow>();		break; // Custom filter, shader, model
		case SampleId::Breaker:	m_curr_sample = std::make_unique<SampleBreaker>();  break; // Collisions, Particles
		case SampleId::Canvas:	m_curr_sample = std::make_unique<SampleCanvas>();   break; // 2D drawing
	}
}

bool App::wantQuit() const {
    return m_curr_sample ? m_curr_sample->wantQuit : false;
}
