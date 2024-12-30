// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/hud/status_lines.hh"

#include "client/gui/text_shadow.hh"

#include "client/globals.hh"


constexpr static ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration;

static float line_offsets[STATUS_COUNT];
static const ImFont *line_fonts[STATUS_COUNT];

static Vec4f line_text_colors[STATUS_COUNT];
static Vec4f line_shadow_colors[STATUS_COUNT];
static std::string line_strings[STATUS_COUNT];
static std::uint64_t line_spawns[STATUS_COUNT];
static float line_fadeouts[STATUS_COUNT];

void status_lines::init(void)
{
    for(unsigned int i = 0U; i < STATUS_COUNT; ++i) {
        line_text_colors[i] = Vec4f::transparent();
        line_shadow_colors[i] = Vec4f::transparent();
        line_strings[i] = std::string();
        line_spawns[i] = UINT64_MAX;
        line_fadeouts[i] = 0.0f;
    }
}

void status_lines::init_late(void)
{
    line_offsets[STATUS_DEBUG] = 64.0f;
    line_offsets[STATUS_HOTBAR] = 40.0f;
}

void status_lines::layout(void)
{
    line_fonts[STATUS_DEBUG] = globals::font_debug;
    line_fonts[STATUS_HOTBAR] = globals::font_chat;

    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(viewport->Size);

    if(!ImGui::Begin("###HUD_StatusLines", nullptr, WINDOW_FLAGS)) {
        ImGui::End();
        return;
    }

    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    for(unsigned int i = 0U; i < STATUS_COUNT; ++i) {
        const auto offset = line_offsets[i] * globals::gui_scale;
        const auto &text = line_strings[i];
        const auto *font = line_fonts[i];

        const auto size = font->CalcTextSizeA(font->FontSize, FLT_MAX, 0.0f, text.c_str(), text.c_str() + text.size());
        const auto pos = ImVec2(0.5f * (viewport->Size.x - size.x), viewport->Size.y - offset);

        const auto spawn = line_spawns[i];
        const auto fadeout = line_fadeouts[i];
        const auto alpha = std::exp(-1.0f * std::pow(1.0e-6f * static_cast<float>(globals::curtime - spawn) / fadeout, 10.0f));

        const auto &color = line_text_colors[i];
        const auto &shadow = line_shadow_colors[i];
        const auto color_U32 = ImGui::GetColorU32(ImVec4(color.get_x(), color.get_y(), color.get_z(), alpha));
        const auto shadow_U32 = ImGui::GetColorU32(ImVec4(shadow.get_x(), shadow.get_y(), shadow.get_z(), alpha));

        text_shadow::layout(text, pos, color_U32, shadow_U32, font, draw_list);
    }

    ImGui::End();
}

void status_lines::set(unsigned int line, const std::string &text, const Vec4f &color, float fadeout)
{
    line_text_colors[line] = Vec4f(color);
    line_shadow_colors[line] = Vec4f(color * 0.1f);
    line_strings[line] = std::string(text);
    line_spawns[line] = globals::curtime;
    line_fadeouts[line] = fadeout;
}

void status_lines::unset(unsigned int line)
{
    line_text_colors[line] = Vec4f::transparent();
    line_shadow_colors[line] = Vec4f::transparent();
    line_strings[line] = std::string();
    line_spawns[line] = UINT64_C(0);
    line_fadeouts[line] = 0.0f;
}
