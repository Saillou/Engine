#pragma once
#include "TrainGame/App/Level/Level.hpp"
#include "TrainGame/App/Level/Sandbox/UI/Ui.hpp"
//#include "Sample/App/Ui.hpp"

namespace Thomas
{
    class Sandbox : public Level, private Event::Subscriber
    {
    public:
        Sandbox();

        void load() override;
        void unload() override;
        void update(float dt) override;

    private:
        void _on_key_pressed(const CommonEvents::KeyPressed& evt);
        Ui m_ui;
    };
}