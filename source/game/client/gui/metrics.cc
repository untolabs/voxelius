// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/gui/metrics.hh"

#include "client/gui/text_shadow.hh"

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
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));

    if(!ImGui::Begin("###GUI_Metrics", nullptr, WINDOW_FLAGS)) {
        ImGui::End();
        return;
    }

    const auto text_U32 = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    const auto shadow_U32 = ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 1.0f));

    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    ImVec2 position = ImVec2(8.0f, 8.0f);

    const auto &io = ImGui::GetIO();
    const auto ui_framerate = io.Framerate;
    const auto ui_frametime = io.DeltaTime * 1000.0f;

    if(client_game::vertical_sync) {
        text_shadow::layout(fmt::format("{:.02f} FPS [{:.02f} ms] [VSYNC]", ui_framerate, ui_frametime),
            position, text_U32, shadow_U32, globals::font_debug, draw_list);
        position.y += 1.5f * globals::font_debug->FontSize;
    }
    else {
        text_shadow::layout(fmt::format("{:.02f} FPS [{:.02f} ms]", ui_framerate, ui_frametime),
            position, text_U32, shadow_U32, globals::font_debug, draw_list);
        position.y += 1.5f * globals::font_debug->FontSize;
    }

    text_shadow::layout(fmt::format("World drawcalls: {}", globals::num_drawcalls),
        position, text_U32, shadow_U32, globals::font_debug, draw_list);
    position.y += 1.5f * globals::font_debug->FontSize;

    text_shadow::layout(fmt::format("World triangles: {}", globals::num_triangles),
        position, text_U32, shadow_U32, globals::font_debug, draw_list);
    position.y += 1.5f * globals::font_debug->FontSize;

    text_shadow::layout(fmt::format("GL_VERSION: {}", gl_version),
        position, text_U32, shadow_U32, globals::font_debug, draw_list);
    position.y += 1.5f * globals::font_debug->FontSize;
    
    text_shadow::layout(fmt::format("GL_RENDERER: {}", gl_renderer),
        position, text_U32, shadow_U32, globals::font_debug, draw_list);
    position.y += 1.5f * globals::font_debug->FontSize;

    const ChunkCoord &cpos = view::position.chunk;
    const VoxelCoord vpos = WorldCoord::to_voxel(view::position);

    text_shadow::layout(fmt::format("voxel: [{} {} {}]", vpos[0], vpos[1], vpos[2]),
        position, text_U32, shadow_U32, globals::font_debug, draw_list);
    position.y += 1.5f * globals::font_debug->FontSize;

    text_shadow::layout(fmt::format("chunk: [{} {} {}]", cpos[0], cpos[1], cpos[2]),
        position, text_U32, shadow_U32, globals::font_debug, draw_list);
    position.y += 1.5f * globals::font_debug->FontSize;

    ImGui::End();
}
