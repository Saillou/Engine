#include "TextEngine.hpp"
#include "ShaderManager.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <freetype/freetype.h>

// - Public (Static)
void TextEngine::Write(const std::string& text, float x, float y, float scale, const glm::vec4& color) {
    _getInstance()._render(text, x, y, scale, color);
}

glm::vec2 TextEngine::Measure(const std::string& text, float scale) {
    return _getInstance()._measure(text, scale);
}

glm::vec2 TextEngine::MeasureRel(const std::string& text, float scale) {
    if (_getInstance().m_w <= 0 || _getInstance().m_h <= 0)
        return glm::vec2(1.0f, 1.0f);

    glm::vec2 size = Measure(text, scale);
    return glm::vec2(size.x / _getInstance().m_w, size.y / _getInstance().m_h);
}

void TextEngine::SetViewport(int x, int y, int width, int height) {
    _getInstance().m_text_shader
        .use()
        .set("projection", glm::ortho((float)x, (float)width, (float)y, (float)height));

    _getInstance().m_x = x;
    _getInstance().m_y = y;
    _getInstance().m_w = width;
    _getInstance().m_h = height;
}

void TextEngine::GetViewport(int& x, int& y, int& width, int& height) {
    x      = _getInstance().m_x;
    y      = _getInstance().m_y;
    width  = _getInstance().m_w;
    height = _getInstance().m_h;
}

// - Private
TextEngine& TextEngine::_getInstance() {
    static TextEngine engine;
    return engine;
}

// Instance (lazy init, hit on 1st)
TextEngine::TextEngine():
    m_vbo(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
    m_text_shader(ShaderManager::Get(CookType::Text))
{
    // TODO: Font choice (here hardcoded)
    FT_Library ft;
    FT_Init_FreeType(&ft);

    FT_Face face;
    FT_New_Face(ft, "C:/Windows/Fonts/Calibri.ttf", 0, &face);
    FT_Set_Pixel_Sizes(face, 0, (unsigned)NOMINAL_HEIGHT);

    // load first 128 characters of ASCII set and put them in gl_textures
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        FT_Load_Char(face, c, FT_LOAD_RENDER);

        m_char_map.emplace(c, _Character {
            std::make_unique<Texture>(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            ),
            glm::ivec2(face->glyph->bitmap.width,  face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left,   face->glyph->bitmap_top),
            glm::ivec2(face->glyph->advance.x,     face->glyph->advance.y),
            glm::ivec2(face->glyph->metrics.width, face->glyph->metrics.height + face->glyph->metrics.vertBearingY)
        });
    }
    Texture::unbind(GL_TEXTURE_2D);

    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // configure VAO/VBO for texture quads
    m_vao.bind();
    m_vbo.bindData(sizeof(float) * 6 * 4);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    m_vbo.unbind();
    m_vao.unbind();
}

glm::vec2 TextEngine::_measure(const std::string& text, float scale)
{
    float x = 0.0f;
    float y = 0.0f;

    // Get the centroid
    float left   = std::numeric_limits<float>::max();
    float right  = std::numeric_limits<float>::min();
    float top    = std::numeric_limits<float>::max();
    float bottom = std::numeric_limits<float>::min();

    // iterate through all characters
    for (const char c : text) {
        const _Character& ch = m_char_map.at(c);
        float xpos = x + (ch.bearing.x) * scale;
        float ypos = y + (ch.bearing.y - ch.size.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        left   = std::min(left,   xpos);
        right  = std::max(right,  xpos + ch.size.x * scale);
        top    = std::min(top,    ypos);
        bottom = std::max(bottom, ypos + ch.size.y * scale);

        x += (ch.advance.x >> 6) * scale;
    }

    return glm::vec2(right - left, bottom - top);
}

// Methods
void TextEngine:: _render(const std::string& text, float x, float y, float scale, const glm::vec4& color) {
    // activate corresponding render state	
    m_text_shader.use().set("textColor", color);

    Texture::activate(GL_TEXTURE0);
    m_vao.bind();

    // iterate through all characters
    for (const char c : text) {
        const _Character& ch = m_char_map.at(c);

        float xpos = x + scale * (ch.bearing.x);
        float ypos = y + scale * (ch.bearing.y - ch.size.y - NOMINAL_HEIGHT * 0.50f);

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        // update VBO for each character
        std::vector<float> vertices = {
            xpos,     ypos + h,   0.0f, 0.0f,
            xpos,     ypos,       0.0f, 1.0f,
            xpos + w, ypos,       1.0f, 1.0f,

            xpos,     ypos + h,   0.0f, 0.0f,
            xpos + w, ypos,       1.0f, 1.0f,
            xpos + w, ypos + h,   1.0f, 0.0f 
        };

        ch.pTexture->bind();

        m_vbo.bindSubData(vertices);
        m_vbo.unbind();

        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance.x >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    m_vao.unbind();
    Texture::unbind(GL_TEXTURE_2D);
}