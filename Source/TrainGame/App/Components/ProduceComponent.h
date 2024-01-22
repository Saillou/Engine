#pragma once

#include <vector>

#include "Component.hpp"
#include "TrainGame/App/Objects/Item.h"

namespace Thomas
{
    class ProduceComponent : public Component
    {
    public:
        ProduceComponent(const Item& item, float timeS, size_t limit);
        ~ProduceComponent();

        void update(float dt) override;

        std::vector<Item> getItems();
        void clearItems(int amount = -1); // -1 for clearing all of the itmes

    private:
        void produce();

        Item    m_item;
        float   m_timeS;
        float   m_timerS;
        size_t  m_limit;
        size_t  m_currentSize;

        ProgressBar m_bar;
    };

} // namespace Thomas