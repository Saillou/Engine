#include "DropComponent.h"

#include <iostream>

namespace Thomas
{

    DropComponent::DropComponent(Transform* transform, const glm::vec3& p1, const glm::vec3& p2)
        : m_transform(transform)
        , m_leftPoint(p1)
        , m_rightPoint(p2)
    {
        m_id = ComponentId::DropComponent;
        m_alive = true;
    }

    DropComponent::~DropComponent()
    {
    }

    void DropComponent::setProduceComponent(ProduceComponent* cmpt)
    {
        m_produce = cmpt;
    }

    void DropComponent::setInventoryComponent(InventoryComponent* cmpt)
    {
        m_inventory = cmpt;
    }

    void DropComponent::update(float dt)
    {
        if (!m_produce || !m_inventory)
            return;

        const std::vector<Item> items = m_produce->getItems();
        if (items.empty())
            return;
        
        // TODO: check all objects with inventory here using ObjectManager
        // for now we use shitty method

        const Transform& pos = m_inventory->getPosition();

        if (pos.position.x >= m_transform->position.x + m_leftPoint.x
            && pos.position.x <= m_transform->position.x + m_rightPoint.x
            && pos.position.y >= m_transform->position.y + m_leftPoint.y
            && pos.position.y <= m_transform->position.y + m_rightPoint.y)
        {
            for (size_t i = 0; i < items.size(); i++)
            {
                if (m_inventory->addItem(items[i]))
                    m_produce->clearItems(1);
            }
        }
    }
} // namespace Thomas