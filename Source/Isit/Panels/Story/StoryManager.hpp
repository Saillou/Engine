#pragma once

#include <Engine/Events/Events.hpp>
#include <Engine/Events/CommonEvents.hpp>

#include "AnimationManager.hpp"
#include "StoryEngine/StoryWritter.hpp"

struct StoryManager
{
    virtual ~StoryManager() = default;

    static StoryManager& Instance() {
        struct StoryShared :StoryManager {
            StoryShared() = default;
        };

        static std::shared_ptr<StoryManager> story = std::make_shared<StoryShared>();
        return *story;
    }

    void set_page(const std::string& page) {
        m_current_page = page;
        _story[m_current_page].reset();
    }
    const std::string& get_page() const {
        return m_current_page;
    }
    StoryEngine::Node& current_node() {
        return _story[m_current_page];
    }
    const StoryEngine::Node& current_node() const {
        return _story.at(m_current_page);
    }
    bool contains(const std::string& name) const {
        return _story.find(name) != _story.cend();
    }

    using Story = std::unordered_map<std::string, StoryEngine::Node>;

private:
    StoryManager();

    std::string m_current_page = "intro";
    StoryEngine::Story _story;
};