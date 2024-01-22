#include "Quad.hpp"

#include "../../../Cookable.hpp"

// - Quad -
Quad::Quad(float x, float y, float w, float h)
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
            .add_var("out", "vec4", "FragColor")
            .add_func("void", "main", "", R"_main_(
                FragColor = texture(quadTexture, TexCoords);
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

void Quad::draw() {
    shader_quad.use();
    drawElements();
}

void Quad::drawElements() {
    quad_mesh.drawElements();
}
