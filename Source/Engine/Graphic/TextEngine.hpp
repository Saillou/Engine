#pragma once

#include "Wrapper/Array.hpp"
#include "Wrapper/Buffer.hpp"
#include "Wrapper/Shader.hpp"
#include "Wrapper/Texture.hpp"

#include <string>
#include <memory>
#include <unordered_map>

#include <glad/glad.h>

// (It's a hidden Singleton)
struct TextEngine {
    static void Write(const std::string& text, float x, float y, float scale, const glm::vec4& color);
    static void SetViewport(int x, int y, int width, int height);

    static void GetViewport(int& x, int& y, int& width, int &height);
    static glm::vec2 Measure(const std::string& text, float scale);
    static glm::vec2 MeasureRel(const std::string& text, float scale);

    static const int NOMINAL_HEIGHT = 64;

private:
    static TextEngine& _getInstance();

    // Sub
    struct _Character {
        std::unique_ptr<Texture> pTexture;
        glm::ivec2 size;    // size of glyph
        glm::ivec2 bearing; // offset from baseline to left/top of glyph
        glm::ivec2 advance; // offset to advance to next glyph
        glm::ivec2 metrics;
    };

    // Instance
    TextEngine();

    glm::vec2 _measure(const std::string& text, float scale);
    void _render(const std::string& text, float x, float y, float scale, const glm::vec4& color);

    Array m_vao;
    Buffer m_vbo;
    Shader m_text_shader;
    std::unordered_map<GLchar, _Character> m_char_map;

    int m_x = 0;
    int m_y = 0;
    int m_w = 1;
    int m_h = 1;
};