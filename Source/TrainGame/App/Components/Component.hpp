#pragma once

namespace Thomas
{

    enum class ComponentId
    {
        None = 0,
        TrainController
    };

    class Component
    {
    public:
        virtual void update(float dt) = 0;
        ComponentId m_id = ComponentId::None;
    };
} // namespace Thomas