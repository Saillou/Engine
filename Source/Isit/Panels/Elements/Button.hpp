#pragma once

#include "Graphics/Sprite.hpp"

struct Rectangle {
    float x = 0.0f;
    float y = 0.0f;
    float w = 0.0f;
    float h = 0.0f;

    bool is_pressed = false;
};

struct Button : public Sprite
{
    Button(const Rectangle& rectangle, const std::string& text, const CanvasShape::Color& primary);

    virtual void draw() override;

protected:
    virtual void onMouseOver() override;
    virtual void onMouseOut() override;
    virtual void onMouseReleased() override;
    virtual void onMousePressed() override;

    virtual bool _hitArea(int x, int y) override;

private:
    CanvasShape::Color _primaryColor;
    CanvasShape::Color _secondaryColor;
    const std::string _text;
    int _x;
    int _y;
    int _w;
    int _h;
    bool _is_over = false;
};