#pragma once

#include <memory>
#include "Sample.hpp"

struct App {
    App();

    void changeSample(const SampleId& sample_id);
    bool wantQuit() const;

private:
    std::unique_ptr<Sample> m_curr_sample;
};
