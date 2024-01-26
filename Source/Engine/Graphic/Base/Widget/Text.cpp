#include "Text.hpp"

Text::Text(const std::string& text, float scale):
    Widget(Tag::Text),
    _texts({ text }),
    foregroundColor(1,1,1,1),
    _scale(scale),
    _size(0,0)
{
    _compute_size();
}

Text::Text(const Block& texts, float scale):
    Widget(Tag::Text),
    _texts(texts),
    foregroundColor(1, 1, 1, 1),
    _scale(scale),
    _size(0,0)
{
    _compute_size();
}

void Text::draw(Scene& scene) {
    float y0 = y();

    for (const std::string& str : _texts) {
        glm::vec2 size = TextEngine::Measure(str, _scale);

        scene.renderer().text(
            str, scene.width() * x(), scene.height() * (1.0f - y0), _scale, foregroundColor
        );

        y0 += size.y/ scene.height();
    }
}

const std::string& Text::getText() const {
    return _texts.front();
}

const Text::Block& Text::getTexts() const {
    return _texts;
}

const glm::vec2& Text::getSize() const {
    return _size;
}

const glm::vec2 Text::getSizeRel() const {
    int x, y, w, h;
    TextEngine::GetViewport(x, y, w, h);

    if (h == 0 || w == 0)
        return { 0,0 };

    return { _size.x / (float)w , _size.y /(float)h };
}

void Text::setText(const std::string& txt) {
    _texts[0] = txt;
    _compute_size();
}

void Text::setTexts(const Block& txt) {
    _texts = txt;
    _compute_size();
}

std::string& Text::at(size_t i) {
    while (_texts.size() <= i)
        _texts.push_back("");

    return _texts[i];
}

std::string& Text::operator[](size_t i) {
    while (_texts.size() <= i)
        _texts.push_back("");

    return _texts[i];
}

void Text::_compute_size() {
    for (const std::string& str : _texts) {
        glm::vec2 size = TextEngine::Measure(str, _scale);
        _size.x = glm::max(_size.x, size.x);
        _size.y += size.y;
    }

    w() = _size.x;
    h() = _size.y;
}


