#include "StoryWritter.hpp"

namespace StoryEngine
{
    Writter::Page::IO::Element& Writter::Page::IO::Element::content(const std::string& content) {
        _pElement->content = content;
        return *this;
    };

    Writter::Page::IO::Element& Writter::Page::IO::Element::condition(StoryEngine::Element::Condition condition) {
        _pElement->condition = condition;
        return *this;
    };

    Writter::Page::IO::Element& Writter::Page::IO::Element::action(StoryEngine::Element::Action action) {
        _pElement->action = action;
        return *this;
    };

    Writter::Page::IO::Element& Writter::Page::IO::Element::target(StoryEngine::Element::Target target) {
        _pElement->target = target;
        return *this;
    };

    Writter::Page::IO& Writter::Page::IO::Element::end() {
        return *_parentIO;
    }

    void Writter::Page::IO::Element::_clear()
    {
    }

    Writter::Page::IO::Element& Writter::Page::IO::begin() {
        _pIO->elements.push_back(StoryEngine::Element{ "" });
        _current_element._pElement = &_pIO->elements.back();
        _current_element._parentIO = this;
        return _current_element;
    }

    Writter::Page& Writter::Page::IO::end() {
        return *_parentPage;
    }

    void Writter::Page::IO::_clear()
    {
        _pIO = nullptr;
        _parentPage = nullptr;
    }

    Writter::Page& Writter::Page::variable(const std::string& name, const std::string& value) {
        _pNode->variables[name] = value;
        return *this;
    }

    Writter::Page::IO& Writter::Page::output() {
        _pNode->ios.push_back(StoryEngine::IO(StoryEngine::IO::tag::Output, {}));
        _current_io._pIO = &_pNode->ios.back();
        _current_io._parentPage = this;
        return _current_io;
    }
    Writter::Page::IO& Writter::Page::input() {
        _pNode->ios.push_back(StoryEngine::IO(StoryEngine::IO::tag::Input, {}));
        _current_io._pIO = &_pNode->ios.back();
        _current_io._parentPage = this;
        return _current_io;
    }

    void Writter::Page::_clear()
    {
        _name.clear();
        _pNode = nullptr;
    }

    Writter::Page& Writter::page(const std::string& name) {
        _current_page._clear();

        _current_page._name = name;
        _current_page._pNode = &_container[name]; // create node if not existing
        return _current_page;
    }

    Writter::Writter(Story& s) :_container(s)
    {
    }

}; // !Namespace Story Engine