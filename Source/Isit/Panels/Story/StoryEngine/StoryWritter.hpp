#pragma once

#include "StoryEngine.hpp"

namespace StoryEngine 
{
    struct Writter {
        Writter(Story& s);

        struct Page {
            friend Writter;

            struct IO {
                friend Page;

                struct Element {
                    friend IO;

                    Element& content(const std::string& content);
                    Element& condition(StoryEngine::Element::Condition condition);
                    Element& action(StoryEngine::Element::Action action);
                    Element& target(StoryEngine::Element::Target target);
                    IO& end();

                private:
                    void _clear();

                    StoryEngine::Element* _pElement;
                    IO* _parentIO;
                };

                Element& begin();
                Page& end();

            private:
                void _clear();

                StoryEngine::IO* _pIO;
                Page* _parentPage;
                Element _current_element;
            };

            Page& variable(const std::string& name, const std::string& value);
            IO& output();
            IO& input();

        private:
            void _clear();
            std::string _name;
            StoryEngine::Node* _pNode;
            IO _current_io;
        };

        Page& page(const std::string& name);

    private:
        StoryEngine::Story& _container;
        Page _current_page;
    };
};