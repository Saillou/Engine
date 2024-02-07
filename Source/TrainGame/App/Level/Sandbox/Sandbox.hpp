#pragma once
#include "TrainGame/App/Level/Level.hpp"
#include "TrainGame/App/Level/Sandbox/UI/Ui.hpp"
//#include "Sample/App/Ui.hpp"

namespace Thomas
{
    class Sandbox : public Level
    {
    public:
        Sandbox();

        void load() override;
        void unload() override;
        void update(float dt) override;

    private:
        Ui m_ui;
    };
}