#include "App.hpp"

// List of samples
#include "Samples/SampleCube/SampleCube.hpp"
#include "Samples/SamplePong/SamplePong.hpp"
#include "Samples/SampleTrain/SampleTrain.hpp"
#include "Samples/SampleForest/SampleForest.hpp"
#include "Samples/SampleSnow/SampleSnow.hpp"
#include "Samples/SampleBreaker/SampleBreaker.hpp"

// App
App::App():
    m_curr_sample(std::make_unique<SampleSnow>())
{
}

void App::changeSample(const SampleId& sample_id) {
	m_curr_sample = nullptr;

	switch (sample_id)
	{
		case SampleId::Cube:	m_curr_sample = std::make_unique<SampleCube>();		break;
		case SampleId::Pong:	m_curr_sample = std::make_unique<SamplePong>();		break;
		case SampleId::Train:	m_curr_sample = std::make_unique<SampleTrain>();	break;
		case SampleId::Forest:	m_curr_sample = std::make_unique<SampleForest>();	break;
		case SampleId::Snow:	m_curr_sample = std::make_unique<SampleSnow>();		break;
		case SampleId::Breaker:	m_curr_sample = std::make_unique<SampleBreaker>();  break;
	}
}

bool App::wantQuit() const {
    return m_curr_sample ? m_curr_sample->wantQuit : false;
}
