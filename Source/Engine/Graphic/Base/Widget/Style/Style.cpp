#include "Style.hpp"

Style Style::_s_basic_style = Style::CreateBasic();


Style Style::CreateBasic()
{
    Style sRes;

    sRes._hint_x       = 0.0f;
    sRes._hint_y       = 0.0f;
    sRes._hint_w       = 1.0f;
    sRes._hint_h       = 1.0f;
    sRes._opacity      = 1.0f;
    sRes._textSize     = 0.5f;
    sRes._background   = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    sRes._foreground   = glm::vec4(1.0f);
    sRes._borders      = Transparent();
    sRes._contentAlign = BoxAlign { Align::Centered, Align::Centered };

    return sRes;
}

const glm::vec4 Style::Transparent()
{
    return glm::vec4(0);
}

Style Style::operator+(const Style& styleNew) const
{
    // Members
    Style sRes { };
    sRes._hint_x       = _Select(_hint_x      , styleNew._hint_x      );
    sRes._hint_y       = _Select(_hint_y      , styleNew._hint_y      );
    sRes._hint_w       = _Select(_hint_w      , styleNew._hint_w      );
    sRes._hint_h       = _Select(_hint_h      , styleNew._hint_h      );
    sRes._opacity      = _Select(_opacity     , styleNew._opacity     );
    sRes._textSize     = _Select(_textSize    , styleNew._textSize    );
    sRes._background   = _Select(_background  , styleNew._background  );
    sRes._foreground   = _Select(_foreground  , styleNew._foreground  );
    sRes._borders      = _Select(_borders     , styleNew._borders     );
    sRes._contentAlign = _Select(_contentAlign, styleNew._contentAlign);
    return sRes;
}

// Accessors
float           Style::hint_x()       const { return _hint_x      .value_or(*_s_basic_style._hint_x      ); }
float           Style::hint_y()       const { return _hint_y      .value_or(*_s_basic_style._hint_y      ); }
float           Style::hint_w()       const { return _hint_w      .value_or(*_s_basic_style._hint_w      ); }
float           Style::hint_h()       const { return _hint_h      .value_or(*_s_basic_style._hint_h      ); }
float           Style::opacity()      const { return _opacity     .value_or(*_s_basic_style._opacity     ); }
float           Style::textSize()     const { return _textSize    .value_or(*_s_basic_style._textSize    ); }
glm::vec4       Style::background()   const { return _background  .value_or(*_s_basic_style._background  ); }
glm::vec4       Style::foreground()   const { return _foreground  .value_or(*_s_basic_style._foreground  ); }
glm::vec4       Style::borders()      const { return _borders     .value_or(*_s_basic_style._borders     ); }
Style::BoxAlign Style::contentAlign() const { return _contentAlign.value_or(*_s_basic_style._contentAlign); }

bool Style::has_hint_x()       const { return _hint_x      .has_value(); }
bool Style::has_hint_y()       const { return _hint_y      .has_value(); }
bool Style::has_hint_w()       const { return _hint_w      .has_value(); }
bool Style::has_hint_h()       const { return _hint_h      .has_value(); }
bool Style::has_opacity()      const { return _opacity     .has_value(); }
bool Style::has_textSize()     const { return _textSize    .has_value(); }
bool Style::has_background()   const { return _background  .has_value(); }
bool Style::has_foreground()   const { return _foreground  .has_value(); }
bool Style::has_borders()      const { return _borders     .has_value(); }
bool Style::has_contentAlign() const { return _contentAlign.has_value(); }

void Style::set_hint_x(float v)                 { _hint_x = v;       }
void Style::set_hint_y(float v)                 { _hint_y = v;       }
void Style::set_hint_w(float v)                 { _hint_x = v;       }
void Style::set_hint_h(float v)                 { _hint_h = v;       }
void Style::set_opacity(float v)                { _opacity = v;      }
void Style::set_textSize(float v)               { _textSize = v;     }
void Style::set_background(const glm::vec4& v)  { _background = v;   }
void Style::set_foreground(const glm::vec4& v)  { _foreground = v;   }
void Style::set_borders(const glm::vec4& v)     { _borders = v;      }
void Style::set_contentAlign(const BoxAlign& v) { _contentAlign = v; }