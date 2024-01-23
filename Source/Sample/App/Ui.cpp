#include "Ui.hpp"

Ui::Ui(Scene& scene) : 
	m_scene(scene),
    frame_solo_a(Framebuffer::Unique, scene.width(), scene.height()),
    frame_solo_b(Framebuffer::Unique, scene.width(), scene.height()),
    frame_solo_c(Framebuffer::Unique, scene.width(), scene.height()),
    m_frame(Framebuffer::Multisample, scene.width(), scene.height())
{
    // Events
    _subscribe(&Ui::_on_resize);
    _subscribe(&Ui::_draw);

    // Filter
    shader
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
            .add_var("uniform", "sampler2D", "texture_0")
            .add_var("uniform", "sampler2D", "texture_1")
            .add_var("out", "vec4", "FragColor")
            .add_func("void", "main", "", R"_main_(
                FragColor = (1-opacity) * texture(texture_0, TexCoords) + opacity * texture(texture_1, TexCoords);
            )_main_")
        )
        .link();
}

void Ui::_on_resize(const SceneEvents::Resized& size) {
    frame_solo_a.resize(size.width, size.height);
    frame_solo_b.resize(size.width, size.height);
    frame_solo_c.resize(size.width, size.height);
    m_frame.resize(size.width, size.height);
}

void Ui::_draw(const SceneEvents::RenderFinished&) {
    // Ui frame:
    m_frame.bind();
    {
        m_frame.texture().bind();
        glClearColor(0.15f, 0.05f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_frame.texture().unbind();
    }
    m_frame.unbind();

    // Multisample frame to mono
    Framebuffer::Blit(m_scene.framebuffer_main(), frame_solo_a);
    Framebuffer::Blit(m_frame, frame_solo_b);

    // Mix both:
    frame_solo_c.bind();
    glDisable(GL_DEPTH_TEST);
    {
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Texture::activate(GL_TEXTURE0 + 0);
        frame_solo_a.texture().bind();

        Texture::activate(GL_TEXTURE0 + 1);
        frame_solo_b.texture().bind();

        shader.use()
              .set("opacity", 0.5f)
              .set("texture_0", 0)
              .set("texture_1", 1);

        surface.drawElements();

        Texture::deactivate(GL_TEXTURE_2D, GL_TEXTURE0 + 0);
        Texture::deactivate(GL_TEXTURE_2D, GL_TEXTURE0 + 1);
    }

    glEnable(GL_DEPTH_TEST);
    frame_solo_c.unbind();

    // Send back to main
    m_scene.drawFrame(frame_solo_c);
}
