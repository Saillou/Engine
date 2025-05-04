#pragma once

#include "../StoryEngine/StoryWritter.hpp"
#include "../AnimationManager.hpp"

struct Scene_root 
{
    static void Write(StoryEngine::Story& _story)
    {
        StoryEngine::Writter(_story)
            .page("root")
            .variable("debug", "1")
            .variable("last_page", "0")
            .variable("save_count", "0");

        StoryEngine::Writter(_story).page("retry")
            .output().begin().content("Not implemented yet.");

        StoryEngine::Writter(_story).page("load")
            .output().begin().content("Not implemented yet.");
    }
};