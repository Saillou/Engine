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
        void addObjectDeletionPtr(bool* ptr);

        void update(float dt) override;

    private:
        void onMouseHit(const CommonEvents::MouseHit& evt);
        void onMouseClick(const CommonEvents::MouseClicked& evt);

        void construct();
        void cancel();
        // Misha: is there a better way of doing it ?
        Transform* m_transform  = nullptr;
        GridComponent* m_grid   = nullptr;
        bool* m_objectDeletion  = nullptr; // I am not proud of this, there should be ObjectManager who will help us avoid this terriblness
    };

} // namespace Thomas