#pragma once

#include "../StoryEngine/StoryWritter.hpp"
#include "../AnimationManager.hpp"

struct Scene_intro
{
    static void Write(StoryEngine::Story& _story)
    {
        StoryEngine::Writter(_story)
            .page("intro")
            .variable("page_count", "0")
            .output()
                .begin()
                    .content("Hey young one! How did you reached this place ?")
                    .condition([&] { 
                        return _story["intro"].variables["page_count"] == "0"; 
                     })
                .end()
                .begin()
                    .content("Thus, why are you there ?")
                    .condition([&] { 
                        return _story["intro"].variables["page_count"] == "1"; 
                    })
                .end()
                .begin()
                    .content("mmh.")
                    .condition([&] { 
                        return std::stoi(_story["intro"].variables["page_count"]) > 1; 
                    })
                .end()
            .end()
            .input()
                .begin()
                    .action([&]{
                        _story["intro"].variables["page_count"] = std::to_string(
                            std::stoi(_story["intro"].variables["page_count"]) + 1
                        );
                    })
                .end()
                .begin()
                    .content("I want to debug")
                    .target({ StoryEngine::Element::Target::type::Page, "debug" })
                    .condition([&] { 
                        return _story["root"].variables.at("debug") == "1"; 
                     })
                .end()
                .begin()
                    .content("Mmh.. I just entered, the door was opened. Can I go throught?")
                    .target({ StoryEngine::Element::Target::type::Page, "newGame"})
                .end()
                .begin()
                    .content("Arg.. I came back from the dead and want revenge.")
                    .target({ StoryEngine::Element::Target::type::Page, "retry"})
                    .condition([&] { 
                        return _story["dead"].variables["count"] != "0"; 
                    })
                .end()
                .begin()
                    .content("I want to search my memories.")
                    .target({ StoryEngine::Element::Target::type::Page, "load"})
                    .condition([&] { 
                        return _story["root"].variables["save_count"] != "0";
                    })
                .end()
            .end();
    }
};