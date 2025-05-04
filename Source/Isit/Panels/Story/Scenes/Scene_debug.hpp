#pragma once

#include "../StoryEngine/StoryWritter.hpp"
#include "../AnimationManager.hpp"

struct Scene_debug
{
    static void Write(StoryEngine::Story& _story)
    {
        StoryEngine::Writter(_story)
            .page("debug")
                .output()
                    .begin()
                        .action([&] {})
                    .end()
                .end();
    }
};