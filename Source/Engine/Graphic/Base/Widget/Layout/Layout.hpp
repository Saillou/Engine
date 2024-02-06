#pragma once

#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>
#include <queue>

#include "../Widget.hpp"
#include "../Events/WidgetEvents.hpp"
#include "../Style/StyleSheet.hpp"
#include "../../Scene.hpp"
#include "../../../../Events/CommonEvents.hpp"
#include "../../../../Utils/Filter/CopyFilter.hpp"

struct Layout : 
    public Widget
{
    friend struct SceneFrame;

    enum ContainerType {
        Free, Extend, Fit
    };

    // -- Layout --
    Layout(Scene& scene);
    virtual ~Layout() = default;

    // Methods
    void add(std::shared_ptr<Widget>, const std::string& id = "");
    void add(std::shared_ptr<Widget>, float x, float y, const std::string& id = "");
    void clean();

    template <typename WidgetTag>
        std::shared_ptr<WidgetTag> find(const std::string& id);

    // Events
    virtual void _on_mouse_button(const CommonEvents::MouseButton& evt) override;
    virtual void _on_mouse_move(const CommonEvents::MouseMoved& evt) override;

    // Accessors
    Scene& scene();
    StyleSheet& styleSheet();

    void refreshSize();
    int width() const;
    int height() const;

    int OriginalWidth() const;
    int OriginalHeight() const;

protected:
    virtual void draw(Scene&) override;
    virtual void _apply_styleSheet();
    virtual glm::vec2 _compute_hull_size();

    Scene& m_scene;
    StyleSheet m_stylesheet;
    std::vector<std::shared_ptr<Widget>> m_widgets;

    float padding = 0.01f; // todo:  be put in style

private:
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
