#include "Style.hpp"

Style::Style()
    : hovered_background(*this, {1.f,1.f,1.f,1.f})
    , hint_x(*this, 0.0f)
    , hint_y(*this, 0.0f)
    , hint_w(*this, 1.0f)
    , hint_h(*this, 1.0f)
    , textSize(*this, 0.5f)
    , background(*this, {0.f,0.f,0.f,1.f})
    , foreground(*this, {1.f,1.f,1.f,1.f})
    , borders(*this, Transparent())
    , contentAlign(*this, {Align::Centered, Align::Centered})
    , hovered_foreground(*this, {0.f,0.f,0.f,1.f})
    , clicked_background(*this, {0.5f, 0.5f, 0.5f, 1.0f})
    , clicked_foreground(*this, {0.2f, 0.2f, 0.2f, 1.0f})
{
}

const glm::vec4 Style::Transparent()
{
    return glm::vec4(0);
}

Style Style::operator+(const Style& styleNew) const
{
    // Members
    Style sRes = *this;
    sRes.hint_x += styleNew.hint_x;
    sRes.hint_y += styleNew.hint_y;
    sRes.hint_w += styleNew.hint_w;
    sRes.hint_h += styleNew.hint_h;
    sRes.textSize += styleNew.textSize;
    sRes.background += styleNew.background;
    sRes.foreground += styleNew.foreground;
    sRes.borders += styleNew.borders;
    sRes.contentAlign += styleNew.contentAlign;
    sRes.hovered_background += styleNew.hovered_background;
    sRes.hovered_foreground += styleNew.hovered_foreground;
    sRes.clicked_background += styleNew.clicked_background;
    sRes.clicked_foreground += styleNew.clicked_foreground;
    return sRes;
}