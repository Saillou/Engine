#pragma once

#include "Elements/Button.hpp"

#include "../Models/ModelFight.hpp"
#include "../Models/ModelIdle.hpp"
#include "../Models/ModelStory.hpp"

struct Panel : Event::Subscriber {
    struct PanelData 
    {
        ModelFight fight;
        ModelStory story;
        ModelIdle idle;
    };

    virtual ~Panel() {
        _unsubscribeAll();
    }

    virtual void reset() {};
    virtual void draw(CanvasEntity&, const Scene &) = 0;
    virtual void mouse_clicked(const CommonEvents::MouseButton&) {};
    virtual void mouse_moved(const CommonEvents::MouseMoved&) {};

// Statics
    static PanelData& shared_data()
    {
        static std::shared_ptr<PanelData> shared_data = std::make_shared<PanelData>();
        return *shared_data;
    }
};