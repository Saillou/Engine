#include "ProduceComponent.h"

#include <iostream>

namespace Thomas
{
    ProduceComponent::ProduceComponent(const Item& item, float timeS, size_t limit)
        : m_item(item)
        , m_timeS(timeS)
        , m_timerS(0.f)
        , m_limit(limit)
        , m_currentSize(0)
    {
        m_id = ComponentId::ProduceComponent;
        m_alive = true;
    }

    ProduceComponent::~ProduceComponent()
    {
    }

    void ProduceComponent::update(float dt)
    {
        if (m_currentSize < m_limit)
        {
            m_timerS += dt;
        }
        else
        {
            m_timerS = 0.f;
        }

        if (m_timerS >= m_timeS)
        {
            m_timerS = 0.f;
            produce();
        }
    }

    std::vector<Item> ProduceComponent::getItems()
    {
        std::vector<Item> items;
        for (size_t i = 0; i < m_currentSize; i++)
            items.push_back(m_item);

        return items;
    }

    void ProduceComponent::clearItems(int amount)
    {
        if (amount == -1)
        {
            m_currentSize = 0;
            return;
        }

        assert(amount <= m_currentSize && "Invalid amount value.");
        m_currentSize -= amount;
    }

    void ProduceComponent::produce()
    {
        std::cout << "PRODUCED!\n";
        m_currentSize += 1;

        // TODO: add event to force cool animation :)
    }
} // namespace Thomas