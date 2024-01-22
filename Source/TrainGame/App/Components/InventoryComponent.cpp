#include "InventoryComponent.h"

#include <iostream>

namespace Thomas
{

    InventoryComponent::InventoryComponent(Transform* transform)
        : m_transform(transform)
        , m_limit(0)
    {
        m_id = ComponentId::InventoryComponent;
    }

    InventoryComponent::~InventoryComponent()
    {
    }

    void InventoryComponent::setLimit(size_t limit)
    {
        m_limit = limit;

        assert(m_limit >= m_items.size() && "Invalid items size!");
    }

    size_t InventoryComponent::getEmptySize() const
    {
        assert(m_limit >= m_items.size() && "Invalid items size!");
        return m_limit - m_items.size();
    }

    void InventoryComponent::clearInventory()
    {
        m_items.clear();
    }

    bool InventoryComponent::addItem(const Item& item)
    {
        std::cout << "addItem() is called!\n";
        if (m_limit == m_items.size())
            return false;

        m_items.push_back(item);
        std::cout << "adding new item: " << (int)item.type << "\n";
        std::cout << "current items size: " << m_items.size() << "\n";
        return true;
    }

    const std::vector<Item>& InventoryComponent::getItems() const
    {
        return m_items;
    }

    bool InventoryComponent::removeItem(const Item& item)
    {
        auto it = std::find_if(m_items.begin(), m_items.end(), [&](Item i) {return i.type == item.type; });

        if (it == m_items.end())
            return false;

        m_items.erase(it);
        return true;
    }

    void InventoryComponent::update(float dt)
    {
    }


} // namespace Thomas