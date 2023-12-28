#include "TextEngine.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <freetype/freetype.h>

// - Public (Static)
void TextEngine::Write(std::string text, float x, float y, float scale, glm::vec3 color) {
    _getInstance()._render(text, x, y, scale, color);
}

void TextEngine::SetViewport(int x, int y, int width, int height) {
    _getInstance().m_text_shader.set("projection", glm::ortho((float)x, (float)width, (float)y, (float)height));
}

// - Private
TextEngine& TextEngine::_getInstance() {
    static TextEngine engine;
    return engine;
}

// Instance (lazy init, hit on 1st)
TextEngine::TextEngine() {
    // Setup text shader
    m_text_shader
        .attachSource(GL_VERTEX_SHADER, ShaderSource{}
            .add_var("out",     "vec2", "TexCoords")
            .add_var("in",      "vec4", "vertex")
            .add_var("uniform", "mat4", "projection")
            .add_func("void", "main", "", R"_main_(
                TexCoords   = vertex.zw;
                gl_Position = projection * vec4(vertex.xy, 0.5f, 1.0);
            )_main_").str()
        )
        .attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("out", "vec4", "FragColor")
            .add_var("in",  "vec2", "TexCoords")
            .add_var("uniform", "sampler2D", "text")
            .add_var("uniform", "vec3", "textColor")
            .add_func("void", "main", "", R"_main_(
                vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
                FragColor    = vec4(textColor, 1.0) * sampled;
            )_main_").str()
        )
        .link()
        .use();

    // TODO: Font choice (here hardcoded)
    FT_Library ft;
    FT_Init_FreeType(&ft);

    FT_Face face;
    FT_New_Face(ft, "C:/Windows/Fonts/Candara.ttf", 0, &face);
    FT_Set_Pixel_Sizes(face, 0, 48);

    // load first 128 characters of ASCII set and put them in gl_textures
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        FT_Load_Char(face, c, FT_LOAD_RENDER);

        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        _Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        m_char_map[c] = character;
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // configure VAO/VBO for texture quads
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Methods
void TextEngine:: _render(std::string text, float x, float y, float scale, glm::vec3 color) {
    // activate corresponding render state	
    m_text_shader.use();
    m_text_shader.set("textColor", color);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_VAO);

    // iterate through all characters
    for (const char c : text) {
        const _Character ch = m_char_map.at(c);

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}