#include "App.hpp"

// List of samples
#include "SampleCube/SampleCube.hpp"
#include "SampleTrain/SampleTrain.hpp"
#include "SampleForest/SampleForest.hpp"

// App
App::App()
{
    // Start by creating one sample
    m_curr_sample = std::make_unique<SampleForest>();
}

void App::changeSample(const SampleId& sample_id) {
	switch (sample_id)
	{
		case SampleId::Cube:   m_curr_sample = std::make_unique<SampleCube>();   break;
		case SampleId::Forest: m_curr_sample = std::make_unique<SampleForest>(); break;
		case SampleId::Train:  m_curr_sample = std::make_unique<SampleTrain>();  break;
		//case SampleId::Mandala:	m_curr_sample = to be done; break;
		//case SampleId::Pong:		m_curr_sample = to be done; break;
	}
}

bool App::wantQuit() const {
    return m_curr_sample ? m_curr_sample->wantQuit() : false;
}
