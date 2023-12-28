#pragma once

#include "Shaders/Shader.hpp"

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
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

    // Instance
    TextEngine();
    void _render(std::string text, float x, float y, float scale, glm::vec3 color);

    // Instance members
    unsigned int m_VAO;
    unsigned int m_VBO;
    Shader m_text_shader;
    std::unordered_map<GLchar, _Character> m_char_map;
};