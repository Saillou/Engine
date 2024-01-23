#include "Quad.hpp"

#include "../../../Cookable.hpp"

// - Quad -
Quad::Quad(float x, float y, float w, float h):
    _x(x), _y(y), _w(w), _h(h)
{
    shader_quad
        .attachSource(GL_VERTEX_SHADER, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("out", "vec2", "TexCoords")
            .add_func("void", "main", "", R"_main_(
                gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
                float tx = aPos.x > 0 ? 1.0 : 0.0;
                float ty = aPos.y > 0 ? 1.0 : 0.0;
                TexCoords = vec2(tx, ty);
            )_main_")
        )
        .attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "vec2", "TexCoords")
            .add_var("uniform", "sampler2D", "quadTexture")
            .add_var("uniform", "vec4", "background_color")
            .add_var("out", "vec4", "FragColor")
            .add_func("void", "main", "", R"_main_(
                vec2 tex_size = textureSize(quadTexture, 0);
                vec4 bk_color = tex_size.x * tex_size.y > 1 ? texture(quadTexture, TexCoords) : background_color;
                FragColor = bk_color;
            )_main_")
        )
        .link();

    float x1 = x*2.0f - 1.0f;
    float y1 = y*2.0f - 1.0f;

    float x2 = (x+w)*2.0f - 1.0f;
    float y2 = (y+h)*2.0f - 1.0f;

    // Shape
    createQuad(quad_mesh,
        glm::vec3(x1, y2, 0),
        glm::vec3(x1, y1, 0),
        glm::vec3(x2, y1, 0),
        glm::vec3(x2, y2, 0)
    );

    setupGPU(quad_mesh);
}

void Quad::draw(int texture_location) {
    shader_quad.use().set("quadTexture", texture_location);
    drawElements();
}

void Quad::draw(const glm::vec4& background_color) {
    shader_quad.use().set("background_color", background_color);
    drawElements();
}

void Quad::drawElements() {
    quad_mesh.drawElements();
}

float Quad::x() const {
    return _x;
}
float Quad::y() const {
    return _y;
}
float Quad::w() const {
    return _w;
}
float Quad::h() const  {
    return _h;
}