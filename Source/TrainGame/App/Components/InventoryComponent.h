#pragma once

#include <vector>

#include "Component.hpp"
#include "TrainGame/App/Objects/Transform.h"
#include "TrainGame/App/Objects/Item.h"

namespace Thomas
{
    class InventoryComponent : public Component
    {
    public:
        InventoryComponent(Transform* transform);
        ~InventoryComponent();

        void setLimit(size_t limit);
        size_t getEmptySize() const;

        void clearInventory();
        bool addItem(const Item& item);

        const std::vector<Item>& getItems() const;
        
        bool removeItem(const Item& item);

        void update(float dt) override;

        const Transform& getPosition() const { return *m_transform; } // TODO: temporary, delete once ObjectManager is implemented

    private:
        // Misha: is there a better way of doing it ?
        Transform* m_transform  = nullptr;

        size_t              m_limit;
        std::vector<Item>   m_items;
    };

} // namespace Thomas