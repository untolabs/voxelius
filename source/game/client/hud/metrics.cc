// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/hud/metrics.hh"

#include "cmake/config.hh"

#include "shared/entity/grounded.hh"
#include "shared/entity/head.hh"
#include "shared/entity/transform.hh"
#include "shared/entity/velocity.hh"

#include "client/gui/imdraw_ext.hh"

#include "client/game.hh"
#include "client/globals.hh"
#include "client/view.hh"


constexpr static ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNav;

static std::string gl_version = {};
static std::string gl_renderer = {};

void metrics::init(void)
{
    gl_version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    gl_renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
}

void metrics::layout(void)
{
    auto draw_list = ImGui::GetForegroundDrawList();

    // FIXME: maybe use style colors instead of hardcoding?
    auto text_color = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    auto shadow_color = ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 1.0f));

    auto position = ImVec2(8.0f, 8.0f);
    auto y_step = 1.5f * globals::font_debug->FontSize;

    // Draw version
    auto version_line = fmt::format("Voxelius {}", PROJECT_VERSION_STRING);
    imdraw_ext::text_shadow(version_line, position, text_color, shadow_color, globals::font_debug, draw_list);
    position.y += 1.5f * y_step;

    // Draw client-side window framerate metrics
    auto window_framerate = 1.0f / globals::window_frametime_avg;
    auto window_frametime = 1000.0f * globals::window_frametime_avg;
    auto window_fps_line = fmt::format("{:.02f} FPS [{:.02f} ms]", window_framerate, window_frametime);
    imdraw_ext::text_shadow(window_fps_line, position, text_color, shadow_color, globals::font_debug, draw_list);
    position.y += y_step;

    // Draw world rendering metrics
    auto drawcall_line = fmt::format("World: {} DC / {} TRI", globals::num_drawcalls, globals::num_triangles);
    imdraw_ext::text_shadow(drawcall_line, position, text_color, shadow_color, globals::font_debug, draw_list);
    position.y += y_step;

    // Draw OpenGL version string
    auto gl_version_line = fmt::format("GL_VERSION: {}", gl_version);
    imdraw_ext::text_shadow(gl_version_line, position, text_color, shadow_color, globals::font_debug, draw_list);
    position.y += y_step;

    // Draw OpenGL renderer string
    auto gl_renderer_line = fmt::format("GL_RENDERER: {}", gl_renderer);
    imdraw_ext::text_shadow(gl_renderer_line, position, text_color, shadow_color, globals::font_debug, draw_list);
    position.y += 1.5f * y_step;

    const auto &head = globals::registry.get<HeadComponent>(globals::player);
    const auto &transform = globals::registry.get<TransformComponent>(globals::player);
    const auto &velocity = globals::registry.get<VelocityComponent>(globals::player);

    // Draw player voxel position
    auto voxel_position = WorldCoord::to_voxel(transform.position);
    auto voxel_line = fmt::format("voxel: [{} {} {}]", voxel_position.get_x(), voxel_position.get_y(), voxel_position.get_z());
    imdraw_ext::text_shadow(voxel_line, position, text_color, shadow_color, globals::font_debug, draw_list);
    position.y += y_step;

    // Draw player world position
    auto world_line = fmt::format("world: [{} {} {}] [{:.03f} {:.03f} {:.03f}]",
        transform.position.chunk.get_x(), transform.position.chunk.get_y(), transform.position.chunk.get_z(),
        transform.position.local.get_x(), transform.position.local.get_y(), transform.position.local.get_z());
    imdraw_ext::text_shadow(world_line, position, text_color, shadow_color, globals::font_debug, draw_list);
    position.y += y_step;

    // Draw player look angles
    auto angles = Vec3angles::degrees(transform.angles + head.angles);
    auto angle_line = fmt::format("angle: [{: .03f} {: .03f} {: .03f}]", angles.get_x(), angles.get_y(), angles.get_z());
    imdraw_ext::text_shadow(angle_line, position, text_color, shadow_color, globals::font_debug, draw_list);
    position.y += y_step;
}
