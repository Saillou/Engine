#include "CopyFilter.hpp"


CopyFilter::CopyFilter(int width, int height):
    _frame_solo_a(Framebuffer::Unique, width, height),
    _frame_solo_b(Framebuffer::Unique, width, height),
    _frame_solo_c(Framebuffer::Unique, width, height)
{
    _shader        
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
            .add_var("uniform", "float", "opacity")
            .add_var("uniform", "vec4", "background")
            .add_var("uniform", "sampler2D", "texture_0")
            .add_var("uniform", "sampler2D", "texture_1")
            .add_var("out", "vec4", "FragColor")
            .add_func("void", "main", "", R"_main_(
                float ratio = opacity;

                vec4 color_source = texture(texture_0, TexCoords);
                vec4 color_copy = texture(texture_1, TexCoords);

                // Remove background color
                if(color_copy.rgb == background.rgb) {
                    ratio *= background.a;
                }
                    
                FragColor = (1-ratio) * color_source + ratio * color_copy;
            )_main_")
        )
        .link();
}

void CopyFilter::apply(const Framebuffer& fb0, const Framebuffer& fb1, float opacity, const glm::vec4& background) {
    // Multisample frame to mono
    Framebuffer::Blit(fb0, _frame_solo_a);
    Framebuffer::Blit(fb1, _frame_solo_b);

    // Mix both:
    _frame_solo_c.bind();
    glDisable(GL_DEPTH_TEST);
    {
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Texture::activate(GL_TEXTURE0 + 0);
        _frame_solo_a.texture().bind();

        Texture::activate(GL_TEXTURE0 + 1);
        _frame_solo_b.texture().bind();

        _shader.use()
            .set("background", background)
            .set("opacity", opacity)
            .set("texture_0", 0)
            .set("texture_1", 1);

        _surface.drawElements();

        Texture::deactivate(GL_TEXTURE_2D, GL_TEXTURE0 + 0);
        Texture::deactivate(GL_TEXTURE_2D, GL_TEXTURE0 + 1);
    }

    glEnable(GL_DEPTH_TEST);
    _frame_solo_c.unbind();
}

const Framebuffer& CopyFilter::result() const {
    return _frame_solo_c;
}

void CopyFilter::resize(int width, int height) {
    _frame_solo_a.resize(width, height);
    _frame_solo_b.resize(width, height);
    _frame_solo_c.resize(width, height);
}
