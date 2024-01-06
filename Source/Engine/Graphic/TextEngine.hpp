#pragma once

#include "Utils/Array.hpp"
#include "Utils/Buffer.hpp"
#include "Utils/Shader.hpp"
#include "Utils/Texture.hpp"

#include <string>
#include <memory>
#include <unordered_map>

#include <glad/glad.h>

// (It's a hidden Singleton)
struct TextEngine {
    static void Write(std::string text, float x, float y, float scale, glm::vec3 color);
    static void SetViewport(int x, int y, int width, int height);

private:
    static TextEngine& _getInstance();

    // Sub
    struct _Character {
        std::unique_ptr<Texture> pTexture;
        glm::ivec2   size;      // size of glyph
        glm::ivec2   bearing;   // offset from baseline to left/top of glyph
        unsigned int advance;   // horizontal offset to advance to next glyph
    };

    // Instance
    TextEngine();
    void _render(std::string text, float x, float y, float scale, glm::vec3 color);

    Array m_vao;
    Buffer m_vbo;
    Shader m_text_shader;
    std::unordered_map<GLchar, _Character> m_char_map;
};