#pragma once

#include "Elements/Button.hpp"
#include "Elements/Text.hpp"

#include "../Models/ModelFight.hpp"
#include "../Models/ModelIdle.hpp"
#include "../Models/ModelStory.hpp"

struct Panel : Sprite {
    struct PanelData 
    {
        ModelFight fight;
        ModelStory story;
        ModelIdle idle;
    };

    virtual void reset() {};

    // Statics
    static PanelData& shared_data()
    {
        static std::shared_ptr<PanelData> shared_data = std::make_shared<PanelData>();
        return *shared_data;
    }

    // Events
    struct Events : public Event {
        struct FightModelUpdated : public _Base {
            FightModelUpdated() : _Base(_Type::EventFightModelUpdated) {}
        };
        struct StoryModelUpdated : public _Base {
            StoryModelUpdated() : _Base(_Type::EventStoryModelUpdated) {}
        };
        struct IdleModelUpdated : public _Base {
            IdleModelUpdated() : _Base(_Type::EventIdleModelUpdated) { }
        };

    private:
        enum _Type : int {
            EventFightModelUpdated,
            EventStoryModelUpdated,
            EventIdleModelUpdated,
        };
    };
};