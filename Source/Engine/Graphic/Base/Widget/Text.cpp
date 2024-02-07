#include "Text.hpp"

// Text data
Text::Block::Block(const std::string& data) {
    push_back(data);
}

Text::Block::Block(const char* data) {
    push_back(data);
}

Text::Block::Block(const std::initializer_list<std::string>& data) {
    for (const std::string& v : data)
        push_back(v);
}

// Text widget
Text::Text(const Block& texts, float scale):
    Widget(Style::Tag::Text),
    _texts(texts),
    _size(0,0)
{
    if(scale > 0)
        style().set_textSize(scale);

    _compute_size();
}

void Text::draw(Scene& scene) {
    _compute_size();

    Widget::_applyStyle();

    // Centered
    if (style().contentAlign().h == Style::Align::Centered)
        x() += (w() - _size.x) / 2.0f;

    if (style().contentAlign().v == Style::Align::Centered)
        y() += (h() - _size.y) / 2.0f;

    // Draw text
    float y0 = y();
    for (const std::string& str : _texts) {
        glm::vec2 size = TextEngine::Measure(str, style().textSize());

        scene.renderer().text(
            str, scene.width() * x(), scene.height() * (1.0f - y0), style().textSize(), style().foreground()
        );

        y0 += size.y / scene.height();
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
    _size.x = 0.0f;
    _size.y = 0.0f;

    for (const std::string& str : _texts) {
        glm::vec2 size = TextEngine::MeasureRel(str, style().textSize());
        _size.x = glm::max(_size.x, size.x);
        _size.y += size.y;
    }

    w() = _size.x;
    h() = 1.0f * _size.y;
}
