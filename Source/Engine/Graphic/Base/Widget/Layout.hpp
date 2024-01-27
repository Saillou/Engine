#pragma once

#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>
#include <queue>

#include "Style/StyleSheet.hpp"
#include "Widget.hpp"
#include "../Scene.hpp"
#include "../../../Utils/Filter/CopyFilter.hpp"

struct Layout : 
    public Widget
{
    // -- Layout --
    Layout(Scene& scene);
    virtual ~Layout() = default;

    // Methods
    void add(std::shared_ptr<Widget>, const std::string& id = "");
    void add(std::shared_ptr<Widget>, float x, float y, const std::string& id = "");
    void clean();

    template <typename WidgetTag>
    std::shared_ptr<WidgetTag> find(const std::string& id);

    // Accessors
    Scene& scene();
    StyleSheet& styleSheet();

    int width() const;
    int height() const;


protected:
    virtual void draw(Scene&) override;

private:
    Scene& m_scene;
    StyleSheet m_stylesheet;

    std::vector<std::shared_ptr<Widget>> m_widgets;
    std::unordered_map<std::string, std::shared_ptr<Widget>> m_widgets_id;
};

template<typename WidgetTag>
std::shared_ptr<WidgetTag> Layout::find(const std::string& id)
{
    static_assert(std::is_base_of<Widget, WidgetTag>(), "Can't emit non inherited Widget.");

    if (m_widgets_id.find(id) == m_widgets_id.cend())
        return nullptr;

    return std::dynamic_pointer_cast<WidgetTag>(m_widgets_id.at(id));
}
