// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/gui/background.hh"

#include "mathlib/constexpr.hh"
#include "mathlib/vec2f.hh"

#include "client/resource/texture2D.hh"

#include "client/globals.hh"
#include "client/varied_program.hh"


static VariedProgram program = {};
static std::size_t u_texture = {};
static std::size_t u_scale = {};
static GLuint vaobj = {};
static GLuint vbo = {};

static std::shared_ptr<const Texture2D> texture = nullptr;

void background::init(void)
{
    if(!VariedProgram::setup(program, "shaders/background.vert", "shaders/background.frag")) {
        spdlog::critical("background: program setup failed");
        std::terminate();
    }

    u_scale = VariedProgram::add_uniform(program, "u_Scale");
    u_texture = VariedProgram::add_uniform(program, "u_Texture");

    const Vec2f vertices[4] = {
        Vec2f(0.0f, 1.0f),
        Vec2f(0.0f, 0.0f),
        Vec2f(1.0f, 1.0f),
        Vec2f(1.0f, 0.0f),
    };

    glGenVertexArrays(1, &vaobj);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(vaobj);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vec2f), nullptr);
    glVertexAttribDivisor(0, 0);

    texture = resource::load<Texture2D>("textures/gui/background.png", TEXTURE2D_LOAD_VFLIP);

    if(!texture) {
        spdlog::critical("background: texture load failed");
        std::terminate();
    }
}

void background::deinit(void)
{
    texture = nullptr;
    glDeleteVertexArrays(1, &vaobj);
    glDeleteBuffers(1, &vbo);
    VariedProgram::destroy(program);
}

void background::render(void)
{
    if(!VariedProgram::update(program)) {
        spdlog::critical("background: program update failed");
        std::terminate();
    }

    const float scaled_width = 0.75f * static_cast<float>(globals::width / globals::gui_scale);
    const float scaled_height = 0.75f * static_cast<float>(globals::height / globals::gui_scale);
    const float scale_x = scaled_width / static_cast<float>(texture->width);
    const float scale_y = scaled_height / static_cast<float>(texture->height);

    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->handle);

    glUseProgram(program.handle);
    glUniform2f(program.uniforms[u_scale].location, scale_x, scale_y);
    glUniform1i(program.uniforms[u_texture].location, 0); // GL_TEXTURE0

    glBindVertexArray(vaobj);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
