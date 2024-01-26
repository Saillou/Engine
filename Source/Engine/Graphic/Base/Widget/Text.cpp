#include "Text.hpp"

Text::Text(const std::string& text, float scale):
    Widget(Tag::Text),
    _texts({ text }),
    foregroundColor(1,1,1,1),
    _scale(scale),
    _size(TextEngine::MeasureRel(text, _scale))
{
    // ..
}

Text::Text(const std::vector<std::string>& texts, float scale):
    Widget(Tag::Text),
    _texts(texts),
    foregroundColor(1, 1, 1, 1),
    _scale(scale),
    _size(0,0)
{
    for (const std::string& str : _texts) {
        glm::vec2 size = TextEngine::Measure(str, _scale);
        _size.x = glm::max(_size.x, size.x);
        _size.y += size.y;
    }
}

void Text::draw(Scene& scene) {
    float y0 = y;

    for (const std::string& str : _texts) {
        glm::vec2 size = TextEngine::Measure(str, _scale);

        scene.renderer().text(
            str, scene.width() * x, scene.height() * (1.0f - y0), _scale, foregroundColor
        );

        y0 += size.y/ scene.height();
    }
}

const std::string& Text::getText() const {
    return _texts.front();
}

const std::vector<std::string>& Text::getTexts() const {
    return _texts;
}

const glm::vec2& Text::getSize() const {
    return _size;
}

void Text::setText(const std::string& txt) {
    _texts[0] = txt;
}

void Text::setTexts(const std::vector<std::string>& txt) {
    _texts = txt;
}

std::string& Text::at(size_t i) {
    return _texts[i];
}

std::string& Text::operator[](size_t i) {
    return _texts[i];
}


