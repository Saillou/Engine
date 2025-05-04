#pragma once

#include "Graphics/Sprite.hpp"
#include "Graphics/misc.hpp"
#include "Text.hpp"

struct Button : public Sprite
{
    Button(const Rectangle& rectangle, const std::string& text, const CanvasShape::Color& primary);
    Button(const Circle& circle, const std::string& text, const CanvasShape::Color& primary);
    Button(const Text& text);

    virtual void draw() override;

protected:
    virtual void onMouseReleased() override;
    virtual void onMousePressed() override;

    virtual bool _hitArea(int x, int y) override;

private:
    CanvasShape::Color _primaryColor;
    CanvasShape::Color _secondaryColor;
    Rectangle _rect;
    const std::string _text;

    enum _Type {
        _rectangle, _circle
    } _type;
};