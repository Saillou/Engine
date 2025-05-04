#include "StoryEngine.hpp"

namespace StoryEngine
{
    Element::Element(const Action& aAction) : action(aAction) {}
    Element::Element(const char* aContent) : content(aContent) {}
    Element::Element(const char* aContent, const Action& aAction) : content(aContent), action(aAction) {}
    Element::Element(const char* aContent, const Target& aTarget) : content(aContent), target(aTarget) {}
    Element::Element(const char* aContent, const Target& aTarget, const Action& aAction) : content(aContent), target(aTarget) {}
    Element::Element(const char* aContent, const Condition& aCondition) : content(aContent), condition(aCondition) {}
    Element::Element(const char* aContent, const Condition& aCondition, const Action& aAction) : content(aContent), condition(aCondition), action(aAction) {}
    Element::Element(const char* aContent, const Condition& aCondition, const Target& aTarget) : content(aContent), condition(aCondition), target(aTarget) {}
    Element::Element(const char* aContent, const Condition& aCondition, const Target& aTarget, const Action& aAction) : content(aContent), condition(aCondition), target(aTarget), action(aAction) {}

    IO::IO(tag type_, const std::vector<Element>& elements_) : type(type_), elements(elements_) {};
    IO::IO(tag type_, const Element& element_) : type(type_), elements({ element_ }) {};

    IO& Node::current_io() {
        return ios[_i_io];
    }
    bool Node::next() {
        if (_i_io == ios.size() - 1)
            return false;

        _i_io++;
        return true;
    }
    void Node::reset() {
        _i_io = 0;
    }
}; // !Namespace Story Engine