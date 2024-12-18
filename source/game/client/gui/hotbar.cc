// SPDX-License-Identifier: BSD-2-Clause
#include "client/precompiled.hh"
#include "client/gui/hotbar.hh"

#include "common/config.hh"

#include "shared/world/vdef.hh"

#include "client/event/glfw_key.hh"
#include "client/event/glfw_scroll.hh"

#include "client/gui/settings.hh"

#include "client/globals.hh"


constexpr static float ITEM_SIZE = 20.0f;
constexpr static float HOTBAR_PADDING = 2.0f;

constexpr static ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration;

unsigned int hotbar::active_slot = 0U;
Voxel hotbar::slots[HOTBAR_SIZE] = {};

static std::uint64_t slot_spawn = UINT64_MAX;
static std::string slot_text = std::string();
static int hotbar_keys[HOTBAR_SIZE];

static ImU32 get_color_alpha(ImGuiCol style_color, float alpha)
{
    const ImVec4 &color = ImGui::GetStyleColorVec4(style_color);
    return ImGui::GetColorU32(ImVec4(color.x, color.y, color.z, alpha));
}

static void update_hotbar_item(void)
{
    if(hotbar::slots[hotbar::active_slot] == NULL_VOXEL) {
        slot_spawn = UINT64_MAX;
        slot_text = std::string();
        return;
    }

    if(const VoxelInfo *info = vdef::find(hotbar::slots[hotbar::active_slot])) {
        slot_spawn = globals::curtime;
        slot_text = fmt::format("{}/{}", info->name, info->state);
        return;
    }
}

static void on_glfw_key(const GlfwKeyEvent &event)
{
    if((event.action == GLFW_PRESS) && !globals::gui_screen) {
        for(unsigned int i = 0U; i < HOTBAR_SIZE; ++i) {
            if(event.key == hotbar_keys[i]) {
                hotbar::active_slot = i;
                update_hotbar_item();
                return;
            }
        }
    }
}

static void on_glfw_scroll(const GlfwScrollEvent &event)
{
    if(!globals::gui_screen) {
        if(event.dy < 0.0) {
            hotbar::active_slot += 1U;
            hotbar::active_slot %= HOTBAR_SIZE;
            update_hotbar_item();
            return;
        }

        if(event.dy > 0.0) {
            hotbar::active_slot += HOTBAR_SIZE - 1U;
            hotbar::active_slot %= HOTBAR_SIZE;
            update_hotbar_item();
            return;
        }
    }
}

void hotbar::init(void)
{
    hotbar_keys[0] = GLFW_KEY_1;
    hotbar_keys[1] = GLFW_KEY_2;
    hotbar_keys[2] = GLFW_KEY_3;
    hotbar_keys[3] = GLFW_KEY_4;
    hotbar_keys[4] = GLFW_KEY_5;
    hotbar_keys[5] = GLFW_KEY_6;
    hotbar_keys[6] = GLFW_KEY_7;
    hotbar_keys[7] = GLFW_KEY_8;
    hotbar_keys[8] = GLFW_KEY_9;

    Config::add(globals::client_config, "hotbar.key.0", hotbar_keys[0]);
    Config::add(globals::client_config, "hotbar.key.1", hotbar_keys[0]);
    Config::add(globals::client_config, "hotbar.key.3", hotbar_keys[0]);
    Config::add(globals::client_config, "hotbar.key.4", hotbar_keys[0]);
    Config::add(globals::client_config, "hotbar.key.5", hotbar_keys[0]);
    Config::add(globals::client_config, "hotbar.key.6", hotbar_keys[0]);
    Config::add(globals::client_config, "hotbar.key.7", hotbar_keys[0]);
    Config::add(globals::client_config, "hotbar.key.8", hotbar_keys[0]);
    Config::add(globals::client_config, "hotbar.key.9", hotbar_keys[0]);

    settings::add_key_binding(10, settings::KEYBOARD_GAMEPLAY, "hotbar.key.0", hotbar_keys[0]);
    settings::add_key_binding(11, settings::KEYBOARD_GAMEPLAY, "hotbar.key.1", hotbar_keys[1]);
    settings::add_key_binding(12, settings::KEYBOARD_GAMEPLAY, "hotbar.key.2", hotbar_keys[2]);
    settings::add_key_binding(13, settings::KEYBOARD_GAMEPLAY, "hotbar.key.3", hotbar_keys[3]);
    settings::add_key_binding(14, settings::KEYBOARD_GAMEPLAY, "hotbar.key.4", hotbar_keys[4]);
    settings::add_key_binding(15, settings::KEYBOARD_GAMEPLAY, "hotbar.key.5", hotbar_keys[5]);
    settings::add_key_binding(16, settings::KEYBOARD_GAMEPLAY, "hotbar.key.6", hotbar_keys[6]);
    settings::add_key_binding(17, settings::KEYBOARD_GAMEPLAY, "hotbar.key.7", hotbar_keys[7]);
    settings::add_key_binding(18, settings::KEYBOARD_GAMEPLAY, "hotbar.key.8", hotbar_keys[8]);

    globals::dispatcher.sink<GlfwKeyEvent>().connect<&on_glfw_key>();
    globals::dispatcher.sink<GlfwScrollEvent>().connect<&on_glfw_scroll>();
}

void hotbar::layout(void)
{
    ImGui::PushFont(globals::font_chat);

    ImGuiStyle &style = ImGui::GetStyle();
    const float spacing = style.ItemSpacing.y;

    const float item_size = ITEM_SIZE * globals::gui_scale;
    const float hotbar_height = item_size + 2.0f * spacing;
    const float hotbar_width = HOTBAR_SIZE * (item_size + spacing) + spacing;
    const float hotbar_padding = HOTBAR_PADDING * globals::gui_scale;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 window_start = ImVec2(0.5f * viewport->Size.x - 0.5f * hotbar_width, viewport->Size.y - hotbar_height - hotbar_padding);
    const ImVec2 window_size = ImVec2(hotbar_width, hotbar_height);

    ImGui::SetNextWindowPos(window_start);
    ImGui::SetNextWindowSize(window_size);

    if(!ImGui::Begin("###HUD_Hotbar", nullptr, WINDOW_FLAGS)) {
        ImGui::End();
        ImGui::PopFont();
        return;
    }

    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    ImVec2 bg_end = ImVec2(window_start.x + window_size.x, window_start.y + window_size.y);
    draw_list->AddRectFilled(window_start, bg_end, get_color_alpha(ImGuiCol_TabActive, 1.0f));

    ImVec2 slot_start = ImVec2(window_start.x + spacing, window_start.y + spacing);
    ImVec2 slot_end = ImVec2(slot_start.x + item_size, slot_start.y + item_size);
    ImU32 slot_color = get_color_alpha(ImGuiCol_FrameBg, 1.0f);
    ImU32 sel_color = get_color_alpha(ImGuiCol_Text, 1.0f);

    for(unsigned int i = 0U; i < HOTBAR_SIZE; ++i) {
        draw_list->AddRectFilled(slot_start, slot_end, slot_color);
        if(hotbar::active_slot == i)
            draw_list->AddRect(slot_start, slot_end, sel_color, 0.0f, 0, 2.0f * globals::gui_scale);
        slot_start.x += item_size + spacing;
        slot_end.x += item_size + spacing;
    }

    const ImVec2 text_size = ImGui::CalcTextSize(slot_text.c_str(), slot_text.c_str() + slot_text.size());
    const ImVec2 text_pos = ImVec2(0.5f * viewport->Size.x - 0.5f * text_size.x, viewport->Size.y - hotbar_height - 2.0f * text_size.y);
    const ImVec2 shad_pos = ImVec2(text_pos.x + 0.6f * globals::gui_scale, text_pos.y + 0.6f * globals::gui_scale);

    const float fadeout_seconds = 3.0f;
    const float fadeout = std::exp(-1.0f * std::pow(1.0e-6f * static_cast<float>(globals::curtime - slot_spawn) / fadeout_seconds, 10.0f));
    const ImU32 text_col = get_color_alpha(ImGuiCol_Text, fadeout);
    const ImU32 shad_col = get_color_alpha(ImGuiCol_WindowBg, fadeout);

    const ImFont *font = ImGui::GetFont();
    
    ImDrawList *fg_draw = ImGui::GetForegroundDrawList();
    fg_draw->AddText(font, font->FontSize, shad_pos, shad_col, slot_text.c_str(), slot_text.c_str() + slot_text.size());
    fg_draw->AddText(font, font->FontSize, text_pos, text_col, slot_text.c_str(), slot_text.c_str() + slot_text.size());

    ImGui::End();
    ImGui::PopFont();
}
