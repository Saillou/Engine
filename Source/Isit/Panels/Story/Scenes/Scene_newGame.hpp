#pragma once

#include "../StoryEngine/StoryWritter.hpp"
#include "../AnimationManager.hpp"

struct Scene_newGame
{
    static void Write(StoryEngine::Story& _story)
    {
        StoryEngine::Writter(_story).page("newGame")
            .variable("page_count", "0")
            .output()
                .begin()
                    .content("Is it your desire ? You won't be able to come back.")
                    .condition([&] {
                        return _story["newGame"].variables.at("page_count") == "0";
                    })
                .end()
                .begin()
                    .content("Hugh... So?")
                    .condition([&] {
                        return _story["newGame"].variables.at("page_count") == "1";
                    })
                .end()
                .begin()
                    .content("..")
                    .condition([&] {
                        return std::stoi(_story["newGame"].variables.at("page_count")) > 1;
                    })
                .end()
            .end()
            .input()
                .begin()
                    .action([&] {
                        _story["newGame"].variables["page_count"] = std::to_string(
                            std::stoi(_story["newGame"].variables["page_count"]) + 1
                        );
                    })
                .end()
                .begin()
                    .content("Actually... nevermind.")
                    .target({ StoryEngine::Element::Target::type::Page, "intro" })
                .end()
                .begin()
                    .content("Yes - I'm ready!")
                .end()
            .end()
            .output()
                .begin().action([=]
                    {
                        AnimationManager::Instance().Prepare("animation_new_game_0");
                    })
                .end()
            .end()
            .input()
                .begin()
                    .content("Stay calm.")
                    .target({ StoryEngine::Element::Target::type::Panel, "idle" })
                .end()
                .begin()
                    .content("Spit.")
                    .target({ StoryEngine::Element::Target::type::Panel, "fight" })
                .end()
            .end();
    }
};