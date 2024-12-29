// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/hud/status_lines.hh"

#include "client/gui/text_shadow.hh"

#include "client/globals.hh"


constexpr static ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration;
constexpr static float ITEM_PADDING = 40.0f;

static Vec4f item_color = {};
static Vec4f item_shadow = {};
static std::string item_text = std::string();
static std::uint64_t item_spawn = UINT64_C(0);
static float item_fadeout = 0.0f;

static void layout_line(float padding, const std::string &text, const Vec4f &color, const Vec4f &shadow, std::uint64_t spawn, float fadeout, ImFont *font, ImDrawList *draw_list)
{
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    const auto text_size = font->CalcTextSizeA(font->FontSize, FLT_MAX, 0.0f, text.c_str(), text.c_str() + text.size());
    const auto text_pos = ImVec2(0.5f * (viewport->Size.x - text_size.x), viewport->Size.y - globals::gui_scale * padding);

    const auto alpha = std::exp(-1.0f * std::pow(1.0e-6f * static_cast<float>(globals::curtime - spawn) / fadeout, 10.0f));
    const auto color_U32 = ImGui::GetColorU32(ImVec4(color[0], color[1], color[2], alpha));
    const auto shadow_U32 = ImGui::GetColorU32(ImVec4(shadow[0], shadow[1], shadow[2], alpha));

    text_shadow::layout(text, text_pos, color_U32, shadow_U32, font, draw_list);
}

void status_lines::init(void)
{
    item_color = Vec4f::transparent();
    item_shadow = Vec4f::transparent();
    item_text = std::string();
    item_spawn = UINT64_C(0);
    item_fadeout = 0.0f;
}

void status_lines::layout(void)
{
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(viewport->Size);

    if(!ImGui::Begin("###HUD_StatusLines", nullptr, WINDOW_FLAGS)) {
        ImGui::End();
        return;
    }

    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    layout_line(ITEM_PADDING, item_text, item_color, item_shadow, item_spawn, item_fadeout, globals::font_chat, draw_list);

    ImGui::End();
}

void status_lines::set_item(const std::string &text, const Vec4f &color, float fadeout)
{
    item_color = Vec4f(color);
    item_shadow = Vec4f(color * 0.1f);
    item_text = std::string(text);
    item_spawn = globals::curtime;
    item_fadeout = fadeout;
}
