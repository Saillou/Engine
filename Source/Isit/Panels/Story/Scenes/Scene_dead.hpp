#pragma once

#include "../StoryEngine/StoryWritter.hpp"
#include "../AnimationManager.hpp"

struct Scene_dead
{
    static void Write(StoryEngine::Story& _story)
    {
        StoryEngine::Writter(_story)
            .page("dead")
                .variable("count", "0")
                .output()
                    .begin().action([&] {
                        _story["dead"].variables["count"] = std::to_string(
                            std::stoi(_story["dead"].variables["count"]) + 1
                        );
                        AnimationManager::Instance().Prepare("animation_new_game_0");
                    })
                    .end()
                .end()
                .output()
                    .begin().content("A mortal path to eternity, can only meet hardships.")
                .end();
    }
};