#pragma once

#include <list>

#include "Component.hpp"
#include "TrainGame/App/Objects/Transform.h"
#include <Engine/Events/CommonEvents.hpp>
#include "GridComponent.h"

namespace Thomas
{
    class ConstructComponent : public Component, private Event::Subscriber
    {
    public:
        ConstructComponent(Transform* transform);
        ~ConstructComponent();

        void addGridComponent(GridComponent* grid);

        void update(float dt) override;

    private:
        void onMouseHit(const CommonEvents::MouseHit& evt);
        void onKeyPressed(const CommonEvents::KeyPressed& evt);

        void construct();
        // Misha: is there a better way of doing it ?
        Transform* m_transform  = nullptr;
        GridComponent* m_grid   = nullptr;
    };

} // namespace Thomas