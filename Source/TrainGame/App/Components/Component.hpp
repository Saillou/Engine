#pragma once

namespace Thomas
{

    enum class ComponentId
    {
        None = 0,
        TrainController,
        GridComponent,
        ConstructComponent
    };

    class Component
    {
    public:
        virtual void update(float dt) = 0;
        ComponentId m_id = ComponentId::None;

        bool isAlive() const { return m_alive; };

    protected:
        bool m_alive = true;
    };
} // namespace Thomas