#include "TextWidget.hpp"

TextWidget::TextWidget(const std::string& text, float scale):
    _texts({ text }),
    foregroundColor(1,1,1,1),
    _scale(scale),
    _size(TextEngine::MeasureRel(text, _scale))
{
    // ..
}

TextWidget::TextWidget(const std::vector<std::string>& texts, float scale) :
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

void TextWidget::draw(Scene& scene) {
    float y0 = y;

    for (const std::string& str : _texts) {
        glm::vec2 size = TextEngine::Measure(str, _scale);

        scene.renderer().text(
            str, scene.width() * x, scene.height() * (1.0f - y0), _scale, foregroundColor
        );

        y0 += size.y/ scene.height();
    }
}

const std::string& TextWidget::getText() const {
    return _texts.front();
}

const std::vector<std::string>& TextWidget::getTexts() const {
    return _texts;
}

const glm::vec2& TextWidget::getSize() const {
    return _size;
}

void TextWidget::setText(const std::string& txt) {
    _texts[0] = txt;
}

void TextWidget::setTexts(const std::vector<std::string>& txt) {
    _texts = txt;
}

std::string& TextWidget::at(size_t i) {
    return _texts[i];
}

std::string& TextWidget::operator[](size_t i) {
    return _texts[i];
}


