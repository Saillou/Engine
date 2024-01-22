#pragma once

#include "Component.hpp"
#include "TrainGame/App/Objects/Transform.h"

#include "ProduceComponent.h"
#include "InventoryComponent.h"

namespace Thomas
{
    class DropComponent : public Component
    {
    public:
        DropComponent(Transform* transform, const glm::vec3& p1, const glm::vec3& p2);
        ~DropComponent();

        void setProduceComponent(ProduceComponent* cmpt);
        void setInventoryComponent(InventoryComponent* cmpt);

        void update(float dt) override;

    private:
        // Misha: is there a better way of doing it ?
        Transform* m_transform  = nullptr;
        ProduceComponent* m_produce = nullptr;
        InventoryComponent* m_inventory = nullptr;

        glm::vec3 m_leftPoint;
        glm::vec3 m_rightPoint;
    };

} // namespace Thomas